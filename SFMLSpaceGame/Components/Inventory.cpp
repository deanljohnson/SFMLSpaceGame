#include <Components/Inventory.h>
#include <Event.h>
#include <Entity.h>

Inventory::Inventory()
{
	Item i;
	i.type = Item::ItemType::Credits;
	i.credits.amount = 0;

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
		if (i.type == Item::ItemType::Credits)
			return i.credits.amount;
	}

	return 0;
}

void Inventory::SetCredits(int c)
{
	for (auto& i : *this)
	{
		if (i.type == Item::ItemType::Credits)
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