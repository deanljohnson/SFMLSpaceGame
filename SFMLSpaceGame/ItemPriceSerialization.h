#pragma once
#include <Item.h>
#include <cereal/types/unordered_map.hpp>

// specializing to simpilify text serialization output
// See ItemPriceSet.h and ItemPriceLevelSet.h
namespace cereal
{
	template<class Archive, class T, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void save(Archive& ar, const std::unordered_map<ItemType, std::unordered_map<std::string, T>, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(ItemTypeToString[i.first], i.second));
		}
	}

	template<class Archive, class T, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void save(Archive& ar, const std::unordered_map<std::string, T, C, A>& map)
	{
		for (const auto& i : map)
		{
			ar(make_nvp(i.first, i.second));
		}
	}

	template<class Archive, class T, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void load(Archive& ar, std::unordered_map<ItemType, std::unordered_map<std::string, T>, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			ItemType key{ StringToItemType[namePtr] };
			std::unordered_map<std::string, T> value; ar(value);
			hint = map.emplace_hint(hint, std::move(key), move(value));
		}
	}

	template<class Archive, class T, class C, class A,
		traits::EnableIf<traits::is_text_archive<Archive>::value> = traits::sfinae>
	inline void load(Archive& ar, std::unordered_map<std::string, T, C, A>& map)
	{
		map.clear();

		auto hint = map.begin();
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			std::string key = namePtr;
			T value; ar(value);
			hint = map.emplace_hint(hint, move(key), std::move(value));
		}
	}
}