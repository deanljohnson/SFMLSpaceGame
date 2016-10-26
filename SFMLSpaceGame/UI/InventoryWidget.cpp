#include <UI\InventoryWidget.h>
#include <EntityManager.h>
#include <Components\Inventory.h>
#include <SFGUI/Box.hpp>
#include <sstream>

InventoryWidget::InventoryWidget(bool displayCredits) 
{
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_itemTable = sfg::Table::Create();
	m_creditsLabel = sfg::Label::Create("Credits: 0");

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	if (displayCredits)
		box->Pack(m_creditsLabel);
	box->Pack(m_itemTable);

	m_window->Add(box);
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

void InventoryWidget::SetSize(const sf::Vector2i& size) 
{
	m_size = size;

	m_itemTable->RemoveAll();
	m_itemWidgets.clear();

	m_itemTable->SetRowSpacings(2.f);
	m_itemTable->SetColumnSpacings(2.f);

	for (sf::Uint32 j = 0; j < m_size.y; j++)
	{
		for (sf::Uint32 i = 0; i < m_size.x; i++)
		{
			auto item = InventoryItemWidget::Create("trade-icons", nullptr);
			item->SetRequisition({ 50,50 });
			m_itemWidgets.push_back(item);
			m_itemTable->Attach(item, { i, j, 1, 1 });
		}
	}
}

sfg::Widget::Ptr InventoryWidget::GetWidget()
{ 
	return m_window; 
}

void InventoryWidget::SetTarget(EntityID id)
{
	m_targetHandle = EntityManager::Get(id);

	auto& inven = m_targetHandle->GetComponent<Inventory>();
	
	if (m_creditsLabel != nullptr)
	{
		std::stringstream ss;
		ss << "Credits: " << inven.GetCredits();
		m_creditsLabel->SetText(ss.str());
	}

	int i = 0;
	for (auto it = inven.begin(); it != inven.end(); ++it, ++i)
	{
		// Credits are not shown in the inventory
		if (it->type == Item::ItemType::Credits)
		{
			i--;
			continue;
		}
		if (i >= m_size.x * m_size.y)
		{
			std::cerr << "Cannot handle more than " << m_size.x * m_size.y << " items";
			break;
		}

		m_itemWidgets[i]->SetItem(&*it);
	}
}