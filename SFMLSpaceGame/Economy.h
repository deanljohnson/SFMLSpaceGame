#pragma once
#include <ItemPriceSet.h>
#include <EconomyID.h>

class EconomyAgent;

namespace Economy
{
	void Init();

	void AddAgent(const EconomyAgent& agent);
	void AddAgent(const EconomyAgent& agent, const ItemPriceSet& sellPriceSet, const ItemPriceSet& buyPriceSet);
	void RemoveAgent(const EconomyAgent& agent);

	bool Buys(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	bool Sells(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);

	Price GetBuyPrice(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	Price GetSellPrice(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	Price GetBaselinePrice(ItemType itemType, const std::string& detail = Item::NO_DETAIL);

	ItemPriceSet& GetBuyPriceSet(const EconomyID& ident);
	ItemPriceSet& GetSellPriceSet(const EconomyID& ident);

	void SetBuyPrice(const EconomyID& ident, ItemType itemType, Price price);
	void SetBuyPrice(const EconomyID& ident, ItemType itemType, const std::string& detail, Price price);
	void SetSellPrice(const EconomyID& ident, ItemType itemType, Price price);
	void SetSellPrice(const EconomyID& ident, ItemType itemType, const std::string& detail, Price price);

	// Moves the given Item (type and amount) from the source to the target
	void TransferItems(EconomyAgent& source, EconomyAgent& target, const Item& item);

	// Transfer the given Item (type and amount) from the seller to the target
	// and adjust credits of each agent based on the sellers sell amount
	void DoSell(EconomyAgent& seller, EconomyAgent& target, const Item& item);

	// Transfer the given Item (type and amount) from the source to the buyer
	// and adjust credits of each agent based on the buyers buy amount
	void DoBuy(EconomyAgent& source, EconomyAgent& buyer, const Item& item);
}