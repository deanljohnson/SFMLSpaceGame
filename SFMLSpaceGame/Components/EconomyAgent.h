#pragma once
#include "Component.h"
#include <EconomyID.h>
#include <Item.h>
#include <ItemPrice.h>
#include <PriceSupplier.h>
#include <cereal/access.hpp>

class Inventory;

class EconomyAgent : public Component
{
private:
	EconomyID m_id;
	Inventory& m_inventory;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_id);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<EconomyAgent>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_id);
	}

public:
	explicit EconomyAgent(EntityID ent);
	~EconomyAgent();

	EconomyID GetEconomyID() const;
	
	void AddItem(const Item& item);
	void RemoveItem(const Item& item);

	void GiveCredits(unsigned int credits);
	void TakeCredits(unsigned int credits);

	bool Buys(ItemType itemType);
	bool Sells(ItemType itemType);

	unsigned GetBuyPrice(ItemType itemType);
	unsigned GetSellPrice(ItemType itemType);

	void SetBuyPrice(ItemType itemType, Price price);
	void SetSellPrice(ItemType itemType, Price price);

	void SetBuyPrices(std::initializer_list<std::pair<ItemType, Price>> prices);
	void SetSellPrices(std::initializer_list<std::pair<ItemType, Price>> prices);

	PriceSupplier GetBuyPrices();
	PriceSupplier GetSellPrices();
};