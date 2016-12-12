#pragma once

namespace Equipper
{
	void Equip(const Item& item, EntityID id, size_t slot);
	Item Unequip(ItemType type, EntityID id, size_t slot);
	size_t GetNumSlots(ItemType type, EntityID id);
}