#pragma once
#include "Component.h"
#include <EconomyID.h>
#include <Item.h>
#include <PriceSupplier.h>

class Inventory;

class EconomyAgent : public Component
{
private:
	EconomyID m_id;
	Inventory* m_inventory;

public:
	typedef std::pair<ItemType, unsigned> Price;

	EconomyAgent();
	~EconomyAgent();

	virtual void Init() override;

	EconomyID GetEconomyID() const;
	
	void AddItem(const Item& item);
	void RemoveItem(const Item& item);

	bool Buys(ItemType itemType);
	bool Sells(ItemType itemType);

	unsigned GetBuyPrice(ItemType itemType);
	unsigned GetSellPrice(ItemType itemType);

	void SetBuyPrice(ItemType itemType, unsigned price);
	void SetSellPrice(ItemType itemType, unsigned price);

	void SetBuyPrices(std::initializer_list<Price> prices);
	void SetSellPrices(std::initializer_list<Price> prices);

	PriceSupplier GetBuyPrices();
	PriceSupplier GetSellPrices();
};