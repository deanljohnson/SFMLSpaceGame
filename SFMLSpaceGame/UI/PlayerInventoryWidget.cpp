#include <UI\PlayerInventoryWidget.h>
#include <EntityManager.h>
#include <Components\Inventory.h>

PlayerInventoryWidget::PlayerInventoryWidget() 
{
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_itemTable = sfg::Table::Create();

	m_window->Add(m_itemTable);
}

void PlayerInventoryWidget::Update()
{
	if (!m_targetHandle.IsValid())
		return;

	for (auto& item : m_itemWidgets)
	{
		item->Invalidate();
	}
}

void PlayerInventoryWidget::SetSize(const sf::Vector2i& size) 
{
	m_size = size;

	m_itemTable->RemoveAll();

	m_itemTable->SetRowSpacings(2.f);
	m_itemTable->SetColumnSpacings(2.f);

	for (sf::Uint32 j = 0; j < m_size.y; j++)
	{
		for (sf::Uint32 i = 0; i < m_size.x; i++)
		{
			auto window = sfg::Window::Create(sfg::Window::BACKGROUND);
			window->SetRequisition({ 50, 50 });
			m_itemTable->Attach(window, { i, j, 1, 1 });
		}
	}
}

sfg::Widget::Ptr PlayerInventoryWidget::GetWidget()
{ 
	return m_window; 
}

void PlayerInventoryWidget::SetTarget(EntityID id)
{
	m_targetHandle = EntityManager::Get(id);

	auto& inven = m_targetHandle->GetComponent<Inventory>();
	
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
		m_itemWidgets.push_back(item);
		m_itemTable->Attach(item, { sf::Uint32(i % m_size.y), sf::Uint32(i / m_size.x), 1, 1 });
	}
}