#pragma once
#include <UI/ContextMenu.h>

class ItemContextProvider 
{
public:
	typedef std::function<void(Item*, size_t)> EquipHandler;
	typedef std::function<void(Item*, size_t, bool)> HoverHandler;
private:
	std::unordered_map<ItemType, size_t> m_equipSlotCounts;
	EquipHandler m_equipHandler;
	HoverHandler m_hoverHandler;
public:
	void SetEquipSlotCounts(ItemType type, size_t count);
	void SetContextOptions(ContextMenu& contextMenu, Item* item);

	void SetEquipHandler(EquipHandler equipHandler);
	void SetHoverHandler(HoverHandler hoverHandler);
};