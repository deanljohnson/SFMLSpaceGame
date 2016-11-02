#pragma once
#include <ItemPriceSet.h>
#include <EconomyID.h>

class EconomyAgent;

namespace Economy
{
	void Init();

	void AddAgent(const EconomyAgent& agent);
	void RemoveAgent(const EconomyAgent& agent);

	bool Buys(const EconomyID& ident, ItemType itemType);
	bool Sells(const EconomyID& ident, ItemType itemType);
	unsigned GetBuyPrice(const EconomyID& ident, ItemType itemType);
	unsigned GetSellPrice(const EconomyID& ident, ItemType itemType);
	unsigned GetBaselinePrice(ItemType itemType);

	void SetBuyPrice(const EconomyID& ident, ItemType itemType, unsigned price);
	void SetSellPrice(const EconomyID& ident, ItemType itemType, unsigned price);

	void TransferItems(EconomyAgent& source, EconomyAgent& target, const Item& item);
}