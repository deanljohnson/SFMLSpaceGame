#pragma once
#include <ItemPrice.h>
#include <ItemPriceSerialization.h> // included simply to make sure the templated functions inside are defined

struct EconomyRecord;
class EconomyAgent;
class EconomyID;
class FindBestPurchaseJob;
struct FindPurchaseJobResult;
struct FindSaleJobResult;

namespace Economy
{
	void Init();

	void Update();

	void AddAgent(EconomyAgent& agent);
	void RemoveAgent(const EconomyAgent& agent);

	// Gets the price of buying a single item of the given type and detail
	Price GetBuyPrice(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	// Gets the price of selling a single item of the given type and detail
	Price GetSellPrice(const EconomyID& ident, ItemType itemType, const std::string& detail = Item::NO_DETAIL);

	// Finds the best purchase to make within the given 
	// searchRange from the given start. The passed in 
	// filter is passed the target agent, potential profit, 
	// and item type for each possible trade. It can be 
	// used to modify the profit value or to prevent this 
	// trade from being considered. The best purchase is 
	// that which is cheapest in relation to it's average 
	// sell price.
	FindPurchaseJobResult FindBestPurchase(
		const std::string& start = "",
		size_t searchRange = 0,
		std::function<bool(const EconomyAgent&, Price&, ItemType)> filter = nullptr);

	FindSaleJobResult FindBestSale(
		ItemType typeToSell, const std::string& detail, size_t amountToSell,
		const std::string& start = "",
		size_t searchRange = 0,
		std::function<bool(const EconomyAgent&, Price&)> filter = nullptr);

	void EnqueueJob(std::shared_ptr<FindBestPurchaseJob> job);

	// Moves the given Item (type and amount) from the source to the target
	void TransferItems(EconomyAgent& source, EconomyAgent& target, std::shared_ptr<Item> item);

	// Transfer the given Item (type and amount) from the seller to the target
	// and adjust credits of each agent based on the sellers sell amount
	void DoSell(EconomyAgent& seller, EconomyAgent& target, std::shared_ptr<Item> item);

	// Transfer the given Item (type and amount) from the source to the buyer
	// and adjust credits of each agent based on the buyers buy amount
	void DoBuy(EconomyAgent& source, EconomyAgent& buyer, std::shared_ptr<Item> item);

	void LoadFromRecord(const EconomyRecord& record);
}