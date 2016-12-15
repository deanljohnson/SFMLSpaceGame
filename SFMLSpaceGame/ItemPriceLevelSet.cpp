// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <ItemPriceLevelSet.h>

bool ItemPriceLevelSet::HasLevel(ItemType type, const std::string& detail) const
{
	auto it = m_priceLevels.find(type);
	if (it == m_priceLevels.end())
		return false;

	return it->second.find(detail) != it->second.end();
}

ItemPriceLevel& ItemPriceLevelSet::GetLevel(ItemType type, const std::string& detail) 
{
	auto it = m_priceLevels.find(type);
	if (it == m_priceLevels.end())
		throw "The ItemPriceLevelSet did not contain the given item type - check first with HasLevel";

	return it->second.at(detail);
}

void ItemPriceLevelSet::SetLevel(ItemType type, ItemPriceLevel range)
{
	m_priceLevels[type][Item::NO_DETAIL] = range;
}

void ItemPriceLevelSet::SetLevel(ItemType type, const std::string& detail, ItemPriceLevel range)
{
	m_priceLevels[type][detail] = range;
}

ItemPriceLevelSet::iterator ItemPriceLevelSet::begin()
{
	return m_priceLevels.begin();
}

ItemPriceLevelSet::iterator ItemPriceLevelSet::end()
{
	return m_priceLevels.end();
}

const std::unordered_map<std::string, ItemPriceLevel>& ItemPriceLevelSet::operator[](ItemType type)
{
	return m_priceLevels[type];
}

std::string ItemPriceLevelSet::GetTypeName()
{
	return "ItemPriceLevels";
}