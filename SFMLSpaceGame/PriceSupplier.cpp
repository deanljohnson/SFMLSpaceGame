#include "stdafx.h"
#include <PriceSupplier.h>
#include <Economy.h>

PriceSupplier::PriceSupplier()
	: m_econID(EconomyID::GetDefault()), m_buying(false)
{
}

PriceSupplier::PriceSupplier(const EconomyID& econID, bool buying)
	: m_econID(econID), m_buying(buying)
{
}

bool PriceSupplier::HasPriceForType(ItemType type)
{
	if (m_econID.IsDefault())
		return false;

	return true;
}

unsigned PriceSupplier::GetPriceForType(ItemType type)
{
	if (m_buying)
		return Economy::GetBuyPrice(m_econID, type);
	return Economy::GetSellPrice(m_econID, type);
}