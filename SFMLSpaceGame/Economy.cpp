// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Economy.h>
#include <Components/EconomyAgent.h>
#include <Components/Inventory.h> // needed for lambda.
#include <ItemPriceLevelSet.h>
#include <GraphMap.h>
#include <EconomyRecord.h>
#include <Entity.h>
#include <FindBestPurchaseJob.h>
#include <FindBestSaleJob.h>
#include <shared_mutex>

// controls access to the economy GraphMap
std::shared_mutex m_graphLock;
GraphMap<std::string, EconomyAgent*> m_econGraph;

std::shared_ptr<ItemPriceLevelSet> m_defaultPrices;
std::unordered_map<EconomyAgentType, std::shared_ptr<ItemPriceLevelSet>> m_overriddenPrices;
std::unordered_map<EconomyID, EconomyAgent*> m_econToAgentMap;
std::unordered_map<std::string, EconomyAgent*> m_nameToAgentMap;

std::mutex m_findBestPurchaseQueueLock;
std::queue<std::shared_ptr<FindBestPurchaseJob>> m_findBestPurchaseJobs;

namespace
{
	Price ComputePrice(const ItemPriceLevel& priceLevel, size_t curAmount)
	{
		// base cases requiring no calculation
		if (priceLevel.minAmount == priceLevel.maxAmount) return priceLevel.targetPrice;
		if (curAmount < priceLevel.minAmount) return priceLevel.maxPrice;
		if (curAmount > priceLevel.maxAmount) return priceLevel.minPrice;
		if (curAmount == priceLevel.targetAmount) return priceLevel.targetPrice;

		// We have more items then desired, price will be lower
		if (curAmount > priceLevel.targetAmount)
		{
			auto amtSpread = priceLevel.maxAmount - priceLevel.targetAmount;
			auto overage = curAmount - priceLevel.targetAmount;
			auto overageRatio = static_cast<float>(overage) / static_cast<float>(amtSpread);

			auto priceSpread = priceLevel.targetPrice - priceLevel.minPrice;
			auto priceAdjustment = static_cast<Price>(priceSpread * overageRatio);
			return priceLevel.targetPrice - priceAdjustment;
		}
		// We have less items then desired, price will be higher
		else //if (curAmount < priceLevel.targetAmount) // Always true at this point
		{
			auto amtSpread = priceLevel.targetAmount - priceLevel.minAmount;
			auto underage = priceLevel.targetAmount - curAmount;
			auto underageRatio = static_cast<float>(underage) / static_cast<float>(amtSpread);

			auto priceSpread = priceLevel.maxPrice - priceLevel.targetPrice;
			auto priceAdjustment = static_cast<Price>(priceSpread * underageRatio);
			return priceLevel.targetPrice + priceAdjustment;
		}
	}
}

void Economy::Init()
{
	Serializer<> ser;
	auto iplsPtr = ser.Load<ItemPriceLevelSet>("EconomyBase");
	m_defaultPrices = std::shared_ptr<ItemPriceLevelSet>(iplsPtr);
}

void Economy::Update()
{
	// lock the queue
	std::lock_guard<std::mutex> lock(m_findBestPurchaseQueueLock);

	// Spawn a new thread for every job
	while (!m_findBestPurchaseJobs.empty())
	{
		auto& job = m_findBestPurchaseJobs.front();
		m_findBestPurchaseJobs.pop();
		std::thread t(&FindBestPurchaseJob::Do, &*job);
		// when the thread goes out of scope,
		// do not terminate it's execution
		t.detach();
	}
}

void Economy::AddAgent(EconomyAgent& agent)
{
	m_econToAgentMap.emplace(std::make_pair(agent.GetEconomyID(), &agent));

	if (!agent.entity->GetName().empty())
	{
		m_nameToAgentMap.emplace(make_pair(agent.entity->GetName(), &agent));
	}
}

void Economy::RemoveAgent(const EconomyAgent& agent)
{
	m_econToAgentMap.erase(agent.GetEconomyID());

	if (!agent.entity->GetName().empty())
	{
		m_nameToAgentMap.erase(agent.entity->GetName());
	}
}

Price Economy::GetBuyPrice(const EconomyID& ident, ItemType itemType, const std::string& detail)
{
	auto* agent = m_econToAgentMap[ident];
	auto amt = agent->GetAmountOfItem(itemType, detail);
	Price buyPrice;

	// If there exists a price override for this agent type, item type, and item detail
	auto it = m_overriddenPrices.find(ident.agentType);
	if (it != m_overriddenPrices.end() 
		&& it->second->HasLevel(itemType, detail))
	{
		buyPrice = ComputePrice(it->second->GetLevel(itemType, detail), amt);
	}
	else
	{
		buyPrice = ComputePrice(m_defaultPrices->GetLevel(itemType, detail), amt);
	}

	// buy prices are biased to always be less than normal,
	// which matches with the expectation that when
	// you are buying something you are trying to 
	// get the best deal
	return buyPrice * .85f;
}

Price Economy::GetSellPrice(const EconomyID& ident, ItemType itemType, const std::string& detail)
{
	auto* agent = m_econToAgentMap[ident];
	auto amt = agent->GetAmountOfItem(itemType, detail);
	Price sellPrice;

	// If there exists a price override for this agent type, item type, and item detail
	auto it = m_overriddenPrices.find(ident.agentType);
	if (it != m_overriddenPrices.end()
		&& it->second->HasLevel(itemType, detail))
	{
		sellPrice = ComputePrice(it->second->GetLevel(itemType, detail), amt);
	}
	else
	{
		sellPrice = ComputePrice(m_defaultPrices->GetLevel(itemType, detail), amt);
	}

	// sell prices are biased to always be more than normal,
	// which matches with the expectation that when
	// you are selling something you are trying to 
	// make the most money
	return sellPrice * 1.15f;
}

FindPurchaseJobResult Economy::FindBestPurchase(const std::string& start,
		size_t searchRange, 
		std::function<bool(const EconomyAgent&, Price&, ItemType)> filter)
{
	Price mostPriceDifference = 0;
	ItemType typeToTrade = ItemType::Credits;
	std::string detailToTrade = Item::NO_DETAIL;
	EconomyAgent* agentToTradeWith = nullptr;

	{
		std::shared_lock<std::shared_mutex> lock(m_graphLock);

		m_econGraph.BreadthFirstTraverse(start,
			[searchRange, filter, &mostPriceDifference, &typeToTrade, &detailToTrade, &agentToTradeWith]
		(EconomyAgent*& curAgent) -> bool
		{
			std::shared_lock<std::shared_mutex> l(curAgent->GetInventory().lock);
			// For each item this agent has
			for (auto& item : std::as_const(curAgent->GetInventory()))
			{
				// Credits aren't traded
				if (item->type == ItemType::Credits)
					continue;

				// The price this agent will sell the item for
				Price sellPrice = GetSellPrice(curAgent->GetEconomyID(), item->type, item->GetDetail());
				sellPrice *= item->amount;

				// The average price this item is sold for
				Price avgPrice = m_defaultPrices->GetLevel(item->type, item->GetDetail()).targetPrice;
				avgPrice *= item->amount;

				// If the cost is more than the average, we will simply ignore this agent
				// In the future it would be better to continue considering this agent
				//if (sellPrice > avgPrice)
				//continue;

				Price dif = avgPrice - sellPrice;

				// If the filter returns false, we abort this trade
				if (filter
					&& !filter(*curAgent, dif, item->type))
					continue;

				if (dif > mostPriceDifference)
				{
					mostPriceDifference = dif;
					typeToTrade = item->type;
					detailToTrade = item->GetDetail();
					agentToTradeWith = curAgent;
				}
			}

			return true;
		}, searchRange);
	} // end shared graph lock

	return FindPurchaseJobResult(typeToTrade, detailToTrade, agentToTradeWith);
}

FindSaleJobResult Economy::FindBestSale(ItemType typeToSell, const std::string& detail, size_t amountToSell, const std::string& start, size_t searchRange, std::function<bool(const EconomyAgent&, Price&)> filter)
{
	Price mostPriceDifference = 0;
	EconomyAgent* agentToTradeWith = nullptr;

	{
		std::shared_lock<std::shared_mutex> lock(m_graphLock);

		m_econGraph.BreadthFirstTraverse(start,
			[typeToSell, detail, amountToSell, searchRange, filter, &mostPriceDifference, &agentToTradeWith]
		(EconomyAgent*& curAgent) -> bool
		{
			// The price this agent will buy the item for
			Price purchasePrice = GetBuyPrice(curAgent->GetEconomyID(), typeToSell, detail);
			purchasePrice *= amountToSell;

			// The average price this item is sold for
			Price avgPrice = m_defaultPrices->GetLevel(typeToSell, detail).targetPrice;
			avgPrice *= amountToSell;

			// If the cost is less than the average, we will simply ignore this agent
			// In the future it would be better to continue considering this agent
			if (purchasePrice < avgPrice)
				return false;

			Price dif = purchasePrice - avgPrice;

			// If the filter returns false, we abort this trade
			if (filter
				&& !filter(*curAgent, dif))
				return false;

			if (dif > mostPriceDifference)
			{
				mostPriceDifference = dif;
				agentToTradeWith = curAgent;
			}

			return true;
		}, searchRange);
	} // end shared graph lock

	return FindSaleJobResult(agentToTradeWith);
}

void Economy::EnqueueJob(std::shared_ptr<FindBestPurchaseJob> job)
{
	std::lock_guard<std::mutex> lock(m_findBestPurchaseQueueLock);
	m_findBestPurchaseJobs.push(job);
}

void Economy::TransferItems(EconomyAgent& source, EconomyAgent& target, std::shared_ptr<Item> item)
{
	source.RemoveItem(item);
	target.AddItem(item);
}

void Economy::DoSell(EconomyAgent& seller, EconomyAgent& target, std::shared_ptr<Item> item)
{
	// get the price for a single instance of the item
	Price sellPrice = GetSellPrice(seller.GetEconomyID(), item->type, item->GetDetail());

	TransferItems(seller, target, item);

	// determine the price for the total number of items sold
	sellPrice *= item->amount;

	target.TakeCredits(sellPrice);
	seller.GiveCredits(sellPrice);
}

void Economy::DoBuy(EconomyAgent& source, EconomyAgent& buyer, std::shared_ptr<Item> item)
{
	// get the price for a single instance of the item
	Price buyPrice = GetBuyPrice(buyer.GetEconomyID(), item->type, item->GetDetail());

	TransferItems(source, buyer, item);

	// determine the price for the total number of items bought
	buyPrice *= item->amount;

	buyer.TakeCredits(buyPrice);
	source.GiveCredits(buyPrice);
}

void Economy::LoadFromRecord(const EconomyRecord& record)
{
	std::unique_lock<std::shared_mutex> lock(m_graphLock);

	for (auto& connectionPair : record.tradeConnections)
	{
		if (!m_econGraph.Contains(connectionPair.first))
		{
			m_econGraph.Add(connectionPair.first, m_nameToAgentMap[connectionPair.first]);
		}
		if (!m_econGraph.Contains(connectionPair.second))
		{
			m_econGraph.Add(connectionPair.second, m_nameToAgentMap[connectionPair.second]);
		}

		m_econGraph.Connect(connectionPair.first, connectionPair.second);
	}
}