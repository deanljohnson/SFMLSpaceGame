// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/ItemDragHandler.h>
#include <UI/InventoryItemWidget.h>
#include <WorldConstants.h>

void ItemDragHandler::OnPress(std::shared_ptr<InventoryItemWidget> item) 
{
	m_active = item;
}

void ItemDragHandler::OnRelease(std::shared_ptr<InventoryItemWidget> item) 
{
	m_active = nullptr;
}