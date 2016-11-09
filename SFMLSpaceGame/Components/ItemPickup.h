#pragma once
#include "Component.h"
#include "Physics.h"

class Item;

class ItemPickup : public Component
{
private:
	Physics& m_physics;

	std::vector<Item> m_items;
public:
	explicit ItemPickup(EntityID ent);

	virtual void Update() override;

	void AddItem(const Item& item);
};