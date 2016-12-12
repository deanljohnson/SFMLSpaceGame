#pragma once
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Widgets.hpp>
#include <PriceSupplier.h>
#include <EntityID.h>

class ItemContextProvider;

class InventoryWidget
{
private:
	sfg::ScrolledWindow::Ptr m_scrollWindow;
	sfg::Box::Ptr m_scrollWindowBox;

	PriceSupplier m_prices;
	std::shared_ptr<ItemContextProvider> m_contextProvider;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	int m_selected;

	std::vector<std::function<void(std::shared_ptr<Item>)>> m_itemSelectionChangeCallbacks;

	void Select(int index);
	void OnRightClick(int index);

public:
	InventoryWidget();

	std::shared_ptr<Item> GetSelected();

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);

	void SetPriceSupplier(const PriceSupplier& prices);
	void SetContextProvider(std::shared_ptr<ItemContextProvider> contextProvider);

	void AddItemSelectionChangeCallback(std::function<void(std::shared_ptr<Item>)> callback);
};