#include "stdafx.h"
#include <UI/ContextProvider.h>
#include <Item.h>

void ContextProvider::SetContextOptions(ContextMenu& contextMenu,
										const Item& item)
{
	if (item.IsEquippable())
	{
		contextMenu.AddGroup("Equip",
		{ {"Slot-One", [] {printf("equipping to slot one"); }} });
	}

	contextMenu.AddOption({ "Jettison", [] { printf("Jettison\n"); } });
}