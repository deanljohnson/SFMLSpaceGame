#include "stdafx.h"
#include <UI/ItemContextProvider.h>
#include <Item.h>

void ItemContextProvider::SetEquipSlotCounts(ItemType type, size_t count)
{
	m_equipSlotCounts[type] = count;
}

void ItemContextProvider::SetContextOptions(ContextMenu& contextMenu,
										const Item& item)
{
	if (item.IsEquippable())
	{
		auto it = m_equipSlotCounts.find(item.type);

		if (it != m_equipSlotCounts.end())
		{
			auto count = it->second;

			std::vector<ContextMenu::Option> options;
			for (size_t i = 0; i < count; i++)
			{
				options.push_back(
				{ "Slot " + std::to_string(i),
					[i] { printf("equipping to slot %d", i); }
				});
			}

			contextMenu.AddGroup("Equip", options);
		}
	}

	contextMenu.AddOption({ "Jettison", [] { printf("Jettison\n"); } });
}