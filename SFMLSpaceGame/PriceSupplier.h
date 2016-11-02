#pragma once
#include <Item.h>
#include <EconomyID.h>

class PriceSupplier
{
private:
	EconomyID m_econID;
	bool m_buying;
public:
	PriceSupplier();
	PriceSupplier(const EconomyID& econID, bool buying);

	bool HasPriceForType(ItemType type);
	unsigned GetPriceForType(ItemType type);
};