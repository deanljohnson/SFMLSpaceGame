#include "stdafx.h"
#include <ItemPriceSet.h>

bool ItemPriceSet::HasPrice(ItemType type) 
{
	return m_prices.find(type) != m_prices.end();
}

Price ItemPriceSet::GetPrice(ItemType type)
{
	auto it = m_prices.find(type);
	if (it == m_prices.end())
		return 0;

	return it->second;
}

void ItemPriceSet::SetPrice(ItemType type, Price price)
{
	m_prices[type] = price;
}