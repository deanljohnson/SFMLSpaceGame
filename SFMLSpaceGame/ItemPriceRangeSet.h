#pragma once
#include <unordered_map>
#include <Item.h>
#include <ItemPrice.h>
#include <Range.h>
#include <cereal/types/unordered_map.hpp>

class ItemPriceRangeSet 
{
private:
	std::unordered_map<ItemType, std::unordered_map<std::string, Range<Price>>> m_priceRanges;
public:
	bool HasRange(ItemType type, const std::string& detail = Item::NO_DETAIL);

	Range<Price> GetRange(ItemType type, const std::string& detail = Item::NO_DETAIL);

	void SetRange(ItemType type, Range<Price> range);
	void SetRange(ItemType type, const std::string& detail, Range<Price> range);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("ranges", m_priceRanges));
	}
};

// specializing to simpilify text serialization output
namespace cereal
{
	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void save(Archive& ar, const std::unordered_map<ItemType, std::unordered_map<std::string, Range<Price>>, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(ItemTypeToString[i.first], i.second));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void save(Archive& ar, const std::unordered_map<std::string, Range<Price>, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(i.first, i.second));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void load(Archive& ar, std::unordered_map<ItemType, std::unordered_map<std::string, Range<Price>>, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			ItemType key{ StringToItemType[namePtr] };
			std::unordered_map<std::string, Range<Price>> value; ar(value);
			hint = map.emplace_hint(hint, std::move(key), move(value));
		}
	}

	template<class Archive, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void load(Archive& ar, std::unordered_map<std::string, Range<Price>, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			std::string key = namePtr;
			Range<Price> value; ar(value);
			hint = map.emplace_hint(hint, move(key), std::move(value));
		}
	}
}