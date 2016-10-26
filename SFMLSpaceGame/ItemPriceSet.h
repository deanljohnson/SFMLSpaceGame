#pragma once
#include <unordered_map>
#include <Item.h>

// Stores the prices of different item types
class ItemPriceSet 
{
private:
	std::unordered_map<Item::ItemType, int> m_prices;
public:
	bool HasPrice(Item::ItemType type);

	// Gets the price of the given item type.
	// Returns 0 if the item type is not present
	// in this ItemPriceSet
	int GetPrice(Item::ItemType type);

	void SetPrice(Item::ItemType type, int price);
};