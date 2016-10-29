#include <ItemPriceSet.h>

bool ItemPriceSet::HasPrice(Item::ItemType type) 
{
	return m_prices.find(type) != m_prices.end();
}

ItemPrice ItemPriceSet::GetPrice(Item::ItemType type)
{
	auto it = m_prices.find(type);
	if (it == m_prices.end())
		return ItemPrice();

	return it->second;
}

void ItemPriceSet::SetPrice(Item::ItemType type, const ItemPrice& price)
{
	m_prices[type] = price;
}