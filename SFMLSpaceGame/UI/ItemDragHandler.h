#pragma once

class InventoryItemWidget;

class ItemDragHandler 
{
private:
	std::shared_ptr<InventoryItemWidget> m_active;
public:
	void OnPress(std::shared_ptr<InventoryItemWidget> item);
	void OnRelease(std::shared_ptr<InventoryItemWidget> item);
};