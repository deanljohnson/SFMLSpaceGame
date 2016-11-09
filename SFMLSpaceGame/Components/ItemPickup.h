#pragma once
#include "Component.h"
#include "Physics.h"
#include <cereal/access.hpp>
#include <cereal/types/vector.hpp>

class Item;

class ItemPickup : public Component
{
private:
	Physics& m_physics;

	std::vector<Item> m_items;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_items);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ItemPickup>& construct)
	{
		EntityID selfID;
		ar(selfID);
		construct(selfID);

		ar(construct->m_items);
	}
public:
	explicit ItemPickup(EntityID ent);

	virtual void Update() override;

	void AddItem(const Item& item);
};