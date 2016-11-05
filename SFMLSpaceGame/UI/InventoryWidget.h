#pragma once
#include <EntityID.h>
#include <EntityHandle.h>
#include <ItemPriceSet.h>
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Widgets.hpp>
#include <PriceSupplier.h>

class PriceSupplier;

class InventoryWidget
{
private:
	sfg::ScrolledWindow::Ptr m_scrollWindow;
	sfg::Box::Ptr m_scrollWindowBox;

	PriceSupplier m_prices;

	EntityHandle m_targetHandle;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	int m_selected;

	std::vector<std::function<void(Item*)>> m_itemSelectionChangeCallbacks;

	void Select(int index);
public:
	InventoryWidget();

	void Update();

	Item* GetSelected();

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);

	void SetPriceSupplier(const PriceSupplier& prices);

	void AddItemSelectionChangeCallback(std::function<void(Item*)> callback);
};