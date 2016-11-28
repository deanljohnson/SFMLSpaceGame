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