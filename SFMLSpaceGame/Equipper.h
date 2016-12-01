#pragma once

namespace Equipper
{
	void Equip(Item* item, EntityID id, size_t slot);
	size_t GetNumSlots(ItemType type, EntityID id);
}