#pragma once
#include <unordered_map>
#include <Item.h>
#include <ItemPriceLevel.h>

class ItemPriceLevelSet 
{
private:
	std::unordered_map<ItemType, std::unordered_map<std::string, ItemPriceLevel>> m_priceLevels;
public:
	bool HasLevel(ItemType type, const std::string& detail = Item::NO_DETAIL) const;

	ItemPriceLevel& GetLevel(ItemType type, const std::string& detail = Item::NO_DETAIL);

	void SetLevel(ItemType type, ItemPriceLevel range);
	void SetLevel(ItemType type, const std::string& detail, ItemPriceLevel range);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("buyLevels", m_priceLevels));
	}

	typedef std::unordered_map<ItemType, std::unordered_map<std::string, ItemPriceLevel>>::iterator iterator;

	iterator begin();
	iterator end();

	const std::unordered_map<std::string, ItemPriceLevel>& operator[](ItemType type);

	static std::string GetTypeName();
};