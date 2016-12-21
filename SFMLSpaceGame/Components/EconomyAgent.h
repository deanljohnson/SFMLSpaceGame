#pragma once
#include <EconomyID.h>
#include <Item.h>
#include <cereal/access.hpp>
#include <ItemPrice.h>

class Inventory;
class PriceSupplier;

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
			cereal::make_nvp("econ_id", m_id));
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<EconomyAgent>& construct)
	{
		EntityID selfID;
		EconomyID econID = EconomyID::GetDefault();

		ar(selfID, 
			cereal::make_nvp("econ_id", econID));
		construct(selfID, econID);
	}

	EconomyAgent(EntityID ent, 
				const EconomyID& id);
public:
	explicit EconomyAgent(EntityID ent, EconomyAgentType agentType);
	~EconomyAgent();

	EconomyID GetEconomyID() const;
	
	void AddItem(std::shared_ptr<Item> item);
	void RemoveItem(std::shared_ptr<Item> item);

	void GiveCredits(unsigned int credits);
	void TakeCredits(unsigned int credits);

	size_t GetAmountOfItem(ItemType type, const std::string& detail);

	Price GetBuyPrice(ItemType itemType, const std::string& detail = Item::NO_DETAIL);
	Price GetSellPrice(ItemType itemType, const std::string& detail = Item::NO_DETAIL);

	PriceSupplier GetBuyPrices();
	PriceSupplier GetSellPrices();

	Inventory& GetInventory();
};