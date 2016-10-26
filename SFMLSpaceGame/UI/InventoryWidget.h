#pragma once
#include <EntityID.h>
#include <EntityHandle.h>
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Widgets.hpp>

class InventoryWidget
{
private:
	sfg::ScrolledWindow::Ptr m_scrollWindow;
	sfg::Box::Ptr m_scrollWindowBox;

	EntityHandle m_targetHandle;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	int m_selected;

	sf::Vector2i m_size;

	std::vector<std::function<void(Item*)>> m_itemSelectionChangeCallbacks;
public:
	InventoryWidget();

	void Update();

	Item* GetSelected();

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);

	void AddItemSelectionChangeCallback(std::function<void(Item*)> callback);
};