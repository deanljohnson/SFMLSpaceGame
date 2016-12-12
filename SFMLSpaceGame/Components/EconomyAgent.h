#pragma once
#include <EconomyID.h>
#include <Economy.h>
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
		ar(entity.GetID(), 
			cereal::make_nvp("econ_id", m_id), 
			cereal::make_nvp("buy", Economy::GetBuyPriceSet(m_id)), 
			cereal::make_nvp("sell", Economy::GetSellPriceSet(m_id)));
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<EconomyAgent>& construct)
	{
		EntityID selfID;
		EconomyID econID = EconomyID::GetDefault();
		ItemPriceSet buyPrices;
		ItemPriceSet sellPrices;

		ar(selfID, 
			cereal::make_nvp("econ_id", econID),
			cereal::make_nvp("buy", buyPrices),
			cereal::make_nvp("sell", sellPrices));
		construct(selfID, econID, buyPrices, sellPrices);
	}

	EconomyAgent(EntityID ent, 
				const EconomyID& id,
				const ItemPriceSet& buyPrices, 
				const ItemPriceSet& sellPrices);
public:
	explicit EconomyAgent(EntityID ent);
	~EconomyAgent();

	EconomyID GetEconomyID() const;
	
	void AddItem(std::shared_ptr<Item> item);
	void RemoveItem(std::shared_ptr<Item> item);

	void GiveCredits(unsigned int credits);
	void TakeCredits(unsigned int credits);

	bool Buys(ItemType itemType);
	bool Sells(ItemType itemType);

	unsigned GetBuyPrice(ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	unsigned GetSellPrice(ItemType itemType, const std::string& detail = Item::NO_DETAIL);

	void SetBuyPrice(ItemType itemType, Price price);
	void SetBuyPrice(ItemType itemType, const std::string& detail, Price price);
	void SetSellPrice(ItemType itemType, Price price);
	void SetSellPrice(ItemType itemType, const std::string& detail, Price price);

	void SetBuyPrices(std::initializer_list<std::pair<ItemType, Price>> prices);
	void SetSellPrices(std::initializer_list<std::pair<ItemType, Price>> prices);

	PriceSupplier GetBuyPrices();
	PriceSupplier GetSellPrices();
};