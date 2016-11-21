#include "stdafx.h"
#include <Components/Inventory.h>
#include <Event.h>
#include <Entity.h>

Inventory::Inventory(EntityID ent)
	: Component(ent)
{
	Item i{ItemType::Credits, 0};

	m_items.push_back(i);
}

void Inventory::Update()
{
	Event event;
	while (entity->events.Get(EventType::ItemPickup, event, true))
	{
		AddItem(event.itemPickup.item);
	}
}

int Inventory::GetCredits()
{
	for (auto& i : *this)
	{
		if (i.type == ItemType::Credits)
			return i.credits.amount;
	}

	return 0;
}

void Inventory::SetCredits(int c)
{
	for (auto& i : *this)
	{
		if (i.type == ItemType::Credits)
		{
			 i.credits.amount = c;
			 return;
		}
	}
}

void Inventory::AddItem(const Item& item)
{
	bool stacked = false;
	for (auto& i : *this)
	{
		if (i.type == item.type)
		{
			i.Stack(item);
			stacked = true;
		}
	}

	if (!stacked)
		m_items.push_back(item);
}

void Inventory::RemoveItem(const Item& item)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i].type == item.type)
		{
			if (item.GetAmount() >= m_items[i].GetAmount())
				m_items.erase(m_items.begin() + i);
			else
				m_items[i].SetAmount(m_items[i].GetAmount() - item.GetAmount());

			return;
		}
	}
}