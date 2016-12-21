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

GraphMap<std::string, EconomyAgent*> m_econGraph;
std::shared_ptr<ItemPriceLevelSet> m_defaultPrices;
std::unordered_map<EconomyAgentType, std::shared_ptr<ItemPriceLevelSet>> m_overriddenPrices;
std::unordered_map<EconomyID, EconomyAgent*> m_econToAgentMap;
std::unordered_map<std::string, EconomyAgent*> m_nameToAgentMap;

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

std::pair<ItemType, EconomyAgent*> Economy::FindBestPurchase(const std::string& start, 
		size_t searchRange, 
		std::function<bool(const EconomyAgent&, Price&, ItemType)> filter)
{
	Price mostPriceDifference = 0;
	ItemType typeToTrade = ItemType::Credits;
	EconomyAgent* agentToTradeWith = nullptr;

	m_econGraph.BreadthFirstTraverse(start, 
	[searchRange, filter, &mostPriceDifference, &typeToTrade, &agentToTradeWith]
	(EconomyAgent*& curAgent) -> bool
	{
		// For each item this agent has
		for (auto& item : curAgent->GetInventory())
		{
			// Credits aren't traded
			if (item->type == ItemType::Credits)
				continue;

			// The price this agent will sell the item for
			Price purchasePrice = GetBuyPrice(curAgent->GetEconomyID(), item->type, item->GetDetail());
			purchasePrice *= item->amount;

			// The average price this item is sold for
			Price avgPrice = m_defaultPrices->GetLevel(item->type, item->GetDetail()).targetPrice;
			avgPrice *= item->amount;

			// If the cost is more than the average, we will simply ignore this agent
			// In the future it would be better to continue considering this agent
			if (purchasePrice > avgPrice)
				continue;

			Price dif = avgPrice - purchasePrice;

			// If the filter returns false, we abort this trade
			if (filter 
				&& !filter(*curAgent, dif, item->type))
				continue;

			if (dif > mostPriceDifference)
			{
				mostPriceDifference = dif;
				typeToTrade = item->type;
				agentToTradeWith = curAgent;
			}
		}

		return true;
	}, searchRange);

	return std::make_pair(typeToTrade, agentToTradeWith);
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