#pragma once
#include <unordered_map>
#include <Item.h>
#include <ItemPrice.h>

// Stores the prices of different item types
class ItemPriceSet 
{
private:
	std::unordered_map<ItemType, Price> m_prices;
public:
	bool HasPrice(ItemType type);

	// Gets the price of the given item type.
	// Returns 0 if the item type is not present
	// in this ItemPriceSet
	Price GetPrice(ItemType type);

	void SetPrice(ItemType type, Price price);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_prices);
	}
};