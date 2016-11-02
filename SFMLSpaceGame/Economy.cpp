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
	m_sellPrices.emplace(agent.GetEconomyID(), m_defaultSet);
	m_buyPrices.emplace(agent.GetEconomyID(), m_defaultSet);
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

unsigned Economy::GetBuyPrice(const EconomyID& ident, ItemType itemType)
{
	auto it = m_buyPrices.find(ident);
	if (it == m_buyPrices.end())
		return 0;

	return it->second.GetPrice(itemType);
}

unsigned Economy::GetSellPrice(const EconomyID& ident, ItemType itemType)
{
	auto it = m_sellPrices.find(ident);
	if (it == m_sellPrices.end())
		return 0;

	return it->second.GetPrice(itemType);
}

unsigned Economy::GetBaselinePrice(ItemType itemType)
{
	return m_defaultSet.GetPrice(itemType);
}

void Economy::SetBuyPrice(const EconomyID& ident, ItemType itemType, unsigned price)
{
	auto it = m_buyPrices.find(ident);
	it->second.SetPrice(itemType, price);
}

void Economy::SetSellPrice(const EconomyID& ident, ItemType itemType, unsigned price)
{
	auto it = m_sellPrices.find(ident);
	it->second.SetPrice(itemType, price);
}

void Economy::TransferItems(EconomyAgent& source, EconomyAgent& target, const Item& item)
{
	source.RemoveItem(item);
	target.AddItem(item);
}