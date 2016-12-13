// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <ItemPriceRangeSet.h>

bool ItemPriceRangeSet::HasRange(ItemType type, const std::string& detail)
{
	auto it = m_priceRanges.find(type);
	if (it == m_priceRanges.end())
		return false;

	return it->second.find(detail) != it->second.end();
}

Range<Price> ItemPriceRangeSet::GetRange(ItemType type, const std::string& detail)
{
	auto it = m_priceRanges.find(type);
	if (it == m_priceRanges.end())
		throw "The ItemPriceRangeSet did not contain the given item type - check first with HasRange";

	return it->second[detail];
}

void ItemPriceRangeSet::SetRange(ItemType type, Range<Price> range)
{
	m_priceRanges[type][Item::NO_DETAIL] = range;
}

void ItemPriceRangeSet::SetRange(ItemType type, const std::string& detail, Range<Price> range)
{
	m_priceRanges[type][detail] = range;
}