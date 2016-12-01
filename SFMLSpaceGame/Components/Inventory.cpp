#include "stdafx.h"
#include <Components/Inventory.h>
#include <Event.h>
#include <Entity.h>

Inventory::Inventory(EntityID ent)
	: Component(ent)
{
	m_items.push_back(std::make_shared<CreditsItem>(0));
}

void Inventory::Update()
{
	ItemPickupEvent* itemEvent{nullptr};
	while (itemEvent = entity->events.Get<EventType::ItemPickup>(true))
	{
		AddItem(itemEvent->item);
	}
}

int Inventory::GetCredits()
{
	for (auto& i : *this)
	{
		if (i->type == ItemType::Credits)
			return i->amount;
	}

	return 0;
}

void Inventory::SetCredits(int c)
{
	for (auto& i : *this)
	{
		if (i->type == ItemType::Credits)
		{
			 i->amount = c;
			 return;
		}
	}
}

void Inventory::AddItem(std::shared_ptr<Item> item)
{
	bool stacked = false;
	for (auto& i : *this)
	{
		if (i->type == item->type)
		{
			i->Stack(*item.get());
			stacked = true;
		}
	}

	if (!stacked)
		m_items.push_back(item);
}

void Inventory::RemoveItem(std::shared_ptr<Item> item)
{
	for (size_t i = 0; i < m_items.size(); i++)
	{
		if (m_items[i]->type == item->type)
		{
			if (item->amount >= m_items[i]->amount)
				m_items.erase(m_items.begin() + i);
			else
				m_items[i]->amount = m_items[i]->amount - item->amount;

			return;
		}
	}
}
