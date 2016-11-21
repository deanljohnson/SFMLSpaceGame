#include "stdafx.h"
#include <Economy.h>
#include <Components/EconomyAgent.h>

ItemPriceSet m_defaultSet;
std::unordered_map<EconomyID, ItemPriceSet> m_sellPrices;
std::unordered_map<EconomyID, ItemPriceSet> m_buyPrices;

void Economy::Init()
{
	m_defaultSet.SetPrice(ItemType::Food, 10);
	m_defaultSet.SetPrice(ItemType::FuelCells, 12);
	m_defaultSet.SetPrice(ItemType::Narcotics, 50);
	m_defaultSet.SetPrice(ItemType::Ore, 100);
}

void Economy::AddAgent(const EconomyAgent& agent)
{
	AddAgent(agent, m_defaultSet, m_defaultSet);
}

void Economy::AddAgent(const EconomyAgent& agent, const ItemPriceSet& sellPriceSet, const ItemPriceSet& buyPriceSet)
{
	m_sellPrices.emplace(agent.GetEconomyID(), sellPriceSet);
	m_buyPrices.emplace(agent.GetEconomyID(), buyPriceSet);
}

void Economy::RemoveAgent(const EconomyAgent& agent)
{
	m_sellPrices.erase(agent.GetEconomyID());
	m_buyPrices.erase(agent.GetEconomyID());
}

bool Economy::Buys(const EconomyID& ident, ItemType itemType)
{
	auto it = m_buyPrices.find(ident);
	if (it == m_buyPrices.end())
		return false;

	return it->second.HasPrice(itemType);
}

bool Economy::Sells(const EconomyID& ident, ItemType itemType)
{
	auto it = m_sellPrices.find(ident);
	if (it == m_sellPrices.end())
		return false;

	return it->second.HasPrice(itemType);
}

Price Economy::GetBuyPrice(const EconomyID& ident, ItemType itemType)
{
	auto it = m_buyPrices.find(ident);
	if (it == m_buyPrices.end())
		return 0;

	return it->second.GetPrice(itemType);
}

Price Economy::GetSellPrice(const EconomyID& ident, ItemType itemType)
{
	auto it = m_sellPrices.find(ident);
	if (it == m_sellPrices.end())
		return 0;

	return it->second.GetPrice(itemType);
}

Price Economy::GetBaselinePrice(ItemType itemType)
{
	return m_defaultSet.GetPrice(itemType);
}

ItemPriceSet& Economy::GetBuyPriceSet(const EconomyID& ident)
{
	return m_buyPrices.find(ident)->second;
}

ItemPriceSet& Economy::GetSellPriceSet(const EconomyID& ident)
{
	return m_sellPrices.find(ident)->second;
}

void Economy::SetBuyPrice(const EconomyID& ident, ItemType itemType, Price price)
{
	auto it = m_buyPrices.find(ident);
	it->second.SetPrice(itemType, price);
}

void Economy::SetSellPrice(const EconomyID& ident, ItemType itemType, Price price)
{
	auto it = m_sellPrices.find(ident);
	it->second.SetPrice(itemType, price);
}

void Economy::TransferItems(EconomyAgent& source, EconomyAgent& target, const Item& item)
{
	source.RemoveItem(item);
	target.AddItem(item);
}

void Economy::DoSell(EconomyAgent& seller, EconomyAgent& target, const Item& item)
{
	TransferItems(seller, target, item);

	// get the price for a single instance of the item
	Price sellPrice = GetSellPrice(seller.GetEconomyID(), item.type);

	// determine the price for the total number of items sold
	sellPrice *= item.GetAmount();

	target.TakeCredits(sellPrice);
	seller.GiveCredits(sellPrice);
}

void Economy::DoBuy(EconomyAgent& source, EconomyAgent& buyer, const Item& item)
{
	TransferItems(source, buyer, item);

	// get the price for a single instance of the item
	Price sellPrice = GetBuyPrice(buyer.GetEconomyID(), item.type);

	// determine the price for the total number of items bought
	sellPrice *= item.GetAmount();

	buyer.TakeCredits(sellPrice);
	source.GiveCredits(sellPrice);
}