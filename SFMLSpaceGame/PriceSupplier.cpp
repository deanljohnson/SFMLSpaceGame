// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <PriceSupplier.h>
#include <Economy.h>
#include <Item.h>

PriceSupplier::PriceSupplier()
	: m_econID(EconomyID::GetDefault()), m_buying(false)
{
}

PriceSupplier::PriceSupplier(const EconomyID& econID, bool buying)
	: m_econID(econID), m_buying(buying)
{
}

bool PriceSupplier::HasPriceForType(ItemType type, const std::string& detail = Item::NO_DETAIL)
{
	if (m_econID.IsDefault())
		return false;

	return true;
}

unsigned PriceSupplier::GetPriceForType(ItemType type, const std::string& detail = Item::NO_DETAIL)
{
	if (m_buying)
		return Economy::GetBuyPrice(m_econID, type, detail);
	return Economy::GetSellPrice(m_econID, type, detail);
}