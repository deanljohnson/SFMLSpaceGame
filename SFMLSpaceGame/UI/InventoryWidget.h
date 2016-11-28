#pragma once
#include <EntityID.h>
#include <EntityHandle.h>
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Widgets.hpp>
#include <PriceSupplier.h>

class PriceSupplier;
class ItemDragHandler;

class InventoryWidget
{
private:
	sfg::ScrolledWindow::Ptr m_scrollWindow;
	sfg::Box::Ptr m_scrollWindowBox;

	PriceSupplier m_prices;

	EntityHandle m_targetHandle;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	int m_selected;
	std::shared_ptr<ItemDragHandler> m_dragHandler;

	std::vector<std::function<void(Item*)>> m_itemSelectionChangeCallbacks;

	void Select(int index);
	void OnRightClick(int index);
public:
	InventoryWidget(std::shared_ptr<ItemDragHandler> dragHandler = nullptr);

	void Update();

	Item* GetSelected();

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);

	void SetPriceSupplier(const PriceSupplier& prices);

	void AddItemSelectionChangeCallback(std::function<void(Item*)> callback);
};