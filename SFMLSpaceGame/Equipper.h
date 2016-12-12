#pragma once

namespace Equipper
{
	void Equip(std::shared_ptr<Item> item, EntityID id, size_t slot);
	std::shared_ptr<Item> Unequip(ItemType type, EntityID id, size_t slot);
	size_t GetNumSlots(ItemType type, EntityID id);
}