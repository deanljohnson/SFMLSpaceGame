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
	bool HasPrice(ItemType type, const std::string& detail = Item::NO_DETAIL);

	// Gets the price of the given item type.
	// Returns 0 if the item type is not present
	// in this ItemPriceSet
	Price GetPrice(ItemType type, const std::string& detail = Item::NO_DETAIL);

	void SetPrice(ItemType type, Price price);
	void SetPrice(ItemType type, const std::string& detail, Price price);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(m_prices);
	}
};

// specializing to simpilify text serialization output
namespace cereal
{
	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
		inline void save(Archive& ar, const std::unordered_map<ItemType, std::unordered_map<std::string, Price>, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(ItemTypeToString[i.first], i.second));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void save(Archive& ar, const std::unordered_map<std::string, Price, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(i.first, i.second));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
		inline void load(Archive& ar, std::unordered_map<ItemType, std::unordered_map<std::string, Price>, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			ItemType key{ StringToItemType[namePtr] };
			std::unordered_map<std::string, Price> value; ar(value);
			hint = map.emplace_hint(hint, std::move(key), move(value));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void load(Archive& ar, std::unordered_map<std::string, Price, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			std::string key = namePtr;
			Price value; ar(value);
			hint = map.emplace_hint(hint, move(key), std::move(value));
		}
	}
}