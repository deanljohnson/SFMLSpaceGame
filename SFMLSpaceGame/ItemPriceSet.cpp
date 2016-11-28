#include "stdafx.h"
#include <ItemPriceSet.h>

bool ItemPriceSet::HasPrice(ItemType type, const std::string& detail)
{
	auto it = m_prices.find(type);
	if (it == m_prices.end())
		return false;

	return it->second.find(detail) != it->second.end();
}

Price ItemPriceSet::GetPrice(ItemType type, const std::string& detail)
{
	auto it = m_prices.find(type);
	if (it == m_prices.end())
		return 0;

	return it->second[detail];
}

void ItemPriceSet::SetPrice(ItemType type, Price price)
{
	m_prices[type][Item::NO_DETAIL] = price;
}

void ItemPriceSet::SetPrice(ItemType type, const std::string& detail, Price price)
{
	m_prices[type][detail] = price;
}