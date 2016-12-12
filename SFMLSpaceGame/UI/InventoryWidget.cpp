// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/InventoryWidget.h>
#include <UI/ContextMenu.h>
#include <EntityManager.h>
#include <Components/Inventory.h>
#include <UI/ItemContextProvider.h>
#include <Entity.h>

InventoryWidget::InventoryWidget()
	: m_selected(-1)
{
	m_scrollWindow = sfg::ScrolledWindow::Create();
	m_scrollWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_scrollWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
	m_scrollWindow->AddWithViewport(m_scrollWindowBox);

	m_scrollWindow->SetRequisition({ 300, 300 });
}

Item InventoryWidget::GetSelected()
{
	if (m_selected == -1) return Item();
	return m_itemWidgets[m_selected]->GetItem();
}

sfg::Widget::Ptr InventoryWidget::GetWidget()
{ 
	return m_scrollWindow;
}

void InventoryWidget::SetTarget(EntityID id)
{
	auto targetHandle = EntityManager::Get(id);

	auto& inven = targetHandle->GetComponent<Inventory>();

	// Remove existing items
	for (auto& item : m_itemWidgets)
	{
		m_scrollWindowBox->Remove(item);
	}
	m_itemWidgets.clear();

	int i = 0;
	for (auto it = inven.begin(); it != inven.end(); ++it, ++i)
	{
		// Credits are not shown in the inventory
		if (it->type == ItemType::Credits)
		{
			// we do not our index incremented as we 
			// continue the loop in this case
			i--;
			continue;
		}

		auto item = InventoryItemWidget::Create("trade-icons", *it);

		if (m_prices.HasPriceForType(it->type, it->GetDetail()))
		{
			item->SetItemPrice(m_prices.GetPriceForType(it->type, it->GetDetail()));
		}

		item->GetSignal(InventoryItemWidget::OnLeftClick).Connect(
			[this, i]()
			{
				Select(i);
			});
		item->GetSignal(InventoryItemWidget::OnRightClick).Connect(
			[this, i]()
			{
				OnRightClick(i);
			});

		m_itemWidgets.push_back(item);
		item->SetRequisition({ 0,50 });
		m_scrollWindowBox->Pack(item);
	}

	// this cast to int is required. The compilers tries
	// to convert m_selected to an unsigned value otherwise
	// and in the case that it is -ve, it will give a 
	// value of 1 and screw up this condition
	if (m_selected >= static_cast<int>(m_itemWidgets.size()))
		m_selected = m_itemWidgets.size() - 1;
	if (m_selected >= 0)
		m_itemWidgets[m_selected]->SetSelected(true);
}

void InventoryWidget::SetPriceSupplier(const PriceSupplier& prices)
{
	m_prices = prices;

	for (auto& iw : m_itemWidgets)
	{
		auto item = iw->GetItem();
		if (m_prices.HasPriceForType(item.type,
									item.GetDetail()))
		{
			iw->SetItemPrice(m_prices.GetPriceForType(item.type,
													item.GetDetail()));
		}
	}
}

void InventoryWidget::SetContextProvider(std::shared_ptr<ItemContextProvider> contextProvider) 
{
	m_contextProvider = contextProvider;
}

void InventoryWidget::AddItemSelectionChangeCallback(std::function<void(const Item&)> callback)
{
	m_itemSelectionChangeCallbacks.push_back(callback);
}

void InventoryWidget::Select(int index) 
{
	// -1 is a valid index because it represents having nothing selected
	assert(index >= -1 && index < m_itemWidgets.size());

	// Switch selected item
	if (m_selected != -1) m_itemWidgets[m_selected]->SetSelected(false);
	m_selected = index;

	// if the selection is being cleared
	if (m_selected == -1) return;

	m_itemWidgets[m_selected]->SetSelected(true);

	// Call any registered callbacks
	for (auto& c : m_itemSelectionChangeCallbacks)
	{
		c(m_itemWidgets[m_selected]->GetItem());
	}
}

void InventoryWidget::OnRightClick(int index)
{
	if (m_contextProvider == nullptr)
		return;

	auto contextMenu = GameWindow::GetWindow<ContextMenu>("context_menu");
	contextMenu->SetPosition(GetScreenMouseLocation() - sf::Vector2f{5,5});
	contextMenu->Show(true);

	contextMenu->ClearOptions();

	m_contextProvider->SetContextOptions(*contextMenu, m_itemWidgets[index]->GetItem());
}