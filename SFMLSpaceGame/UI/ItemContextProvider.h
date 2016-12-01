#pragma once
#include <UI/ContextMenu.h>

class ItemContextProvider 
{
private:
	std::unordered_map<ItemType, size_t> m_equipSlotCounts;
public:
	void SetEquipSlotCounts(ItemType type, size_t count);
	void SetContextOptions(ContextMenu& contextMenu, const Item& item);
};