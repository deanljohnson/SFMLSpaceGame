#include <ItemPriceSet.h>

bool ItemPriceSet::HasPrice(Item::ItemType type) 
{
	return m_prices.find(type) != m_prices.end();
}

int ItemPriceSet::GetPrice(Item::ItemType type) 
{
	auto it = m_prices.find(type);
	if (it == m_prices.end())
		return 0;

	return it->second;
}

void ItemPriceSet::SetPrice(Item::ItemType type, int price) 
{
	m_prices[type] = price;
}