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
	Price GetBuyPrice(const EconomyID& ident, ItemType itemType);
	Price GetSellPrice(const EconomyID& ident, ItemType itemType);
	Price GetBaselinePrice(ItemType itemType);

	void SetBuyPrice(const EconomyID& ident, ItemType itemType, Price price);
	void SetSellPrice(const EconomyID& ident, ItemType itemType, Price price);

	// Moves the given Item (type and amount) from the source to the target
	void TransferItems(EconomyAgent& source, EconomyAgent& target, const Item& item);

	// Transfer the given Item (type and amount) from the seller to the target
	// and adjust credits of each agent based on the sellers sell amount
	void DoSell(EconomyAgent& seller, EconomyAgent& target, const Item& item);

	// Transfer the given Item (type and amount) from the source to the buyer
	// and adjust credits of each agent based on the buyers buy amount
	void DoBuy(EconomyAgent& source, EconomyAgent& buyer, const Item& item);
}