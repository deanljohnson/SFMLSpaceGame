#include "stdafx.h"
#include <UI\InventoryWidget.h>
#include <EntityManager.h>
#include <Components\Inventory.h>

InventoryWidget::InventoryWidget() 
	: m_selected(-1)
{
	m_scrollWindow = sfg::ScrolledWindow::Create();
	m_scrollWindowBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_scrollWindow->SetScrollbarPolicy(sfg::ScrolledWindow::VERTICAL_ALWAYS | sfg::ScrolledWindow::HORIZONTAL_NEVER);
	m_scrollWindow->AddWithViewport(m_scrollWindowBox);

	m_scrollWindow->SetRequisition({ 300, 300 });
}

void InventoryWidget::Update()
{
	if (!m_targetHandle.IsValid())
		return;

	for (auto& item : m_itemWidgets)
	{
		item->Invalidate();
	}
}

Item* InventoryWidget::GetSelected()
{
	if (m_selected == -1) return nullptr;
	return m_itemWidgets[m_selected]->GetItem();
}

sfg::Widget::Ptr InventoryWidget::GetWidget()
{ 
	return m_scrollWindow;
}

void InventoryWidget::SetTarget(EntityID id)
{
	m_targetHandle = EntityManager::Get(id);

	auto& inven = m_targetHandle->GetComponent<Inventory>();

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
		if (it->type == Item::ItemType::Credits)
		{
			i--;
			continue;
		}

		auto item = InventoryItemWidget::Create("trade-icons", &*it);

		if (m_prices != nullptr
			&& m_prices->HasPrice(it->type)) 
		{
			item->SetItemPrice(m_prices->GetPrice(it->type));
		}

		item->GetSignal(InventoryItemWidget::OnLeftClick).Connect(
			[this, i]()
			{
				// Switch selected item
				if (m_selected != -1) m_itemWidgets[m_selected]->SetSelected(false);
				m_selected = i;
				m_itemWidgets[m_selected]->SetSelected(true);

				// Call any registered callbacks
				for (auto& c : m_itemSelectionChangeCallbacks)
				{
					c(m_itemWidgets[m_selected]->GetItem());
				}
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

void InventoryWidget::AddItemSelectionChangeCallback(std::function<void(Item*)> callback)
{
	m_itemSelectionChangeCallbacks.push_back(callback);
}