#pragma once
#include <unordered_map>
#include <Item.h>
#include <ItemPrice.h>
#include <cereal/types/unordered_map.hpp>

// Stores the prices of different item types
class ItemPriceSet 
{
private:
	std::unordered_map<ItemType, std::unordered_map<std::string, Price>> m_prices;
public:
	bool HasPrice(ItemType type, const std::string& detail = Item::NO_DETAIL) const;

	// Gets the price of the given item type.
	// Returns 0 if the item type is not present
	// in this ItemPriceSet
	Price GetPrice(ItemType type, const std::string& detail = Item::NO_DETAIL) const;

	void SetPrice(ItemType type, Price price);
	void SetPrice(ItemType type, const std::string& detail, Price price);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("prices", m_prices));
	}

	typedef std::unordered_map<ItemType, std::unordered_map<std::string, Price>>::iterator iterator;

	iterator begin();
	iterator end();
};
