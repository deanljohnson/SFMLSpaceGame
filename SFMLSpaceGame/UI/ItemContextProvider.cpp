// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/ItemContextProvider.h>
#include <Item.h>

void ItemContextProvider::SetEquipSlotCounts(ItemType type, size_t count)
{
	m_equipSlotCounts[type] = count;
}

void ItemContextProvider::SetContextOptions(ContextMenu& contextMenu, std::shared_ptr<Item> item)
{
	if (item->IsEquippable())
	{
		auto it = m_equipSlotCounts.find(item->type);
		
		if (it != m_equipSlotCounts.end())
		{
			auto count = it->second;

			std::vector<ContextMenu::Option> options;
			for (size_t i = 0; i < count; i++)
			{
				options.push_back(
				{ "Slot " + std::to_string(i),
					// Click callback
					[item, i, &contextMenu, this]
					{ 
						if (m_equipHandler != nullptr) m_equipHandler(item, i);
						contextMenu.Close();
					},
					// Start Button hover callback
					[item, i, this]
					{
						if (m_hoverHandler != nullptr) m_hoverHandler(item, i, true);
					},
					// End Button hover callback	
					[item, i, this]
					{
						if (m_hoverHandler != nullptr) m_hoverHandler(item, i, false);
					}
				});
			}

			contextMenu.AddGroup("Equip", options);
		}
	}

	contextMenu.AddOption({ "Jettison", [] { printf("Jettison\n"); } });
}

void ItemContextProvider::SetEquipHandler(EquipHandler equipHandler)
{
	m_equipHandler = equipHandler;
}

void ItemContextProvider::SetHoverHandler(HoverHandler hoverHandler)
{
	m_hoverHandler = hoverHandler;
}