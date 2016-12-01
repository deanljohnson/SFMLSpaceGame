#pragma once
#include <EntityID.h>
#include <EntityHandle.h>
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Widgets.hpp>
#include <PriceSupplier.h>

class ItemContextProvider;

class InventoryWidget
{
private:
	sfg::ScrolledWindow::Ptr m_scrollWindow;
	sfg::Box::Ptr m_scrollWindowBox;

	PriceSupplier m_prices;
	std::shared_ptr<ItemContextProvider> m_contextProvider;

	EntityHandle m_targetHandle;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	int m_selected;

	std::vector<std::function<void(Item*)>> m_itemSelectionChangeCallbacks;

	void Select(int index);
	void OnRightClick(int index);

public:
	InventoryWidget();

	void Update();

	Item* GetSelected();

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);

	void SetPriceSupplier(const PriceSupplier& prices);
	void SetContextProvider(std::shared_ptr<ItemContextProvider> contextProvider);

	void AddItemSelectionChangeCallback(std::function<void(Item*)> callback);
};