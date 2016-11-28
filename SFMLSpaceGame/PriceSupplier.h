#pragma once
#include <EconomyID.h>

enum class ItemType;

class PriceSupplier
{
private:
	EconomyID m_econID;
	bool m_buying;
public:
	PriceSupplier();
	PriceSupplier(const EconomyID& econID, bool buying);

	bool HasPriceForType(ItemType type, const std::string& detail);
	unsigned GetPriceForType(ItemType type, const std::string& detail);
};