// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Economy.h>
#include <Components/EconomyAgent.h>
#include <ItemPriceLevelSet.h>
#include <GraphMap.h>

GraphMap<std::string, std::string> m_econGraph;
std::shared_ptr<ItemPriceLevelSet> m_defaultPrices;
std::unordered_map<EconomyAgentType, std::shared_ptr<ItemPriceLevelSet>> m_overriddenPrices;
std::unordered_map<EconomyID, EconomyAgent*> m_econToAgentMap;

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

	m_econGraph.Add("hello", "a");
	m_econGraph.Add("boo", "b");
	m_econGraph.Remove("hello");
	m_econGraph.Add("hello", "a");
	m_econGraph.Connect("hello", "boo");
	m_econGraph.Disconnect("hello", "boo");
	m_econGraph["boo"] = "b";
	m_econGraph.Connect("hello", "boo");
	//m_econGraph.Remove("boo");

	if (m_econGraph.AreConnected("hello", "boo"))
	{
		printf("bingo\n");
	}

	m_econGraph.Add("hello1", "c");
	m_econGraph.Add("hello2", "d");
	m_econGraph.Connect("hello", "hello1");
	m_econGraph.Connect("hello", "hello2");

	m_econGraph.BreadthFirstTraverse("hello",
		[](std::string& data)
	{
		printf(data.c_str());
		printf("\n");
		return true;
	});


	for (auto it = m_econGraph.NeighborBegin("hello");
		it != m_econGraph.NeighborEnd("hello"); ++it)
	{
		printf(it->c_str());
		printf("\n");
	}
}

void Economy::AddAgent(EconomyAgent& agent)
{
	m_econToAgentMap.emplace(std::make_pair(agent.GetEconomyID(), &agent));
}

void Economy::RemoveAgent(const EconomyAgent& agent)
{
	m_econToAgentMap.erase(agent.GetEconomyID());
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

/*std::pair<EconomyAgent*, ItemType> Economy::GetBestPurchase(EconomyAgentType targetType, 
	std::function<bool(const EconomyAgent&, long&, ItemType)> filter)
{
	Price mostPriceDifference = 0;
	ItemType typeToTrade = ItemType::Credits;
	EconomyAgent* agentToTradeWith = nullptr;
	for (auto& i : m_econToAgentMap)
	{
		// If this agent is not of the right type
		if ((static_cast<int>(i.first.agentType) & static_cast<int>(targetType)) == 0)
			continue;

		for (auto& itemTypeToDetailMap : *m_defaultPrices)
		{
			for (auto& detailToPriceMap : itemTypeToDetailMap.second)
			{
				size_t agentsAmount = i.second->GetAmountOfItem(itemTypeToDetailMap.first, detailToPriceMap.first);

				if (agentsAmount == 0) 
					continue;

				// the cost of buying out this locations stock of a given item
				Price purchasePrice = ComputePrice(detailToPriceMap.second, agentsAmount);
				purchasePrice *= agentsAmount;

				auto defaultPriceLevel = (*m_defaultPrices)[itemTypeToDetailMap.first].at(detailToPriceMap.first);
				Price avgPrice = defaultPriceLevel.targetPrice * agentsAmount;

				// If the cost is more than the average, we will simply ignore this agent
				// In the future it would be better to continue considering this agent
				if (purchasePrice > avgPrice)
					continue;

				long dif = static_cast<long>(avgPrice) - static_cast<long>(purchasePrice);
				if (!filter(*i.second, dif, itemTypeToDetailMap.first))
					continue;

				Price priceDif = dif;
				if (priceDif > mostPriceDifference)
				{
					mostPriceDifference = priceDif;
					typeToTrade = itemTypeToDetailMap.first;
					agentToTradeWith = i.second;
				}
			}
		}
	}
}*/

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