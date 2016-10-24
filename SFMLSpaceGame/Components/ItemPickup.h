#pragma once
#include "Component.h"
#include "Physics.h"

class Item;

class ItemPickup : public Component
{
private:
	Physics* m_physics{ nullptr };

	std::vector<Item> m_items;
public:
	virtual void Init() override;
	virtual void Update() override;

	void AddItem(const Item& item);
};