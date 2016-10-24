#include <UI\PlayerInventoryWidget.h>

PlayerInventoryWidget::PlayerInventoryWidget() 
{
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_itemTable = sfg::Table::Create();

	m_window->Add(m_itemTable);
}

void PlayerInventoryWidget::SetSize(const sf::Vector2i& size) 
{
	m_itemTable->SetRowSpacings(2.f);
	m_itemTable->SetColumnSpacings(2.f);

	for (sf::Uint32 j = 0; j < size.y; j++)
	{
		for (sf::Uint32 i = 0; i < size.x; i++)
		{
			auto window = sfg::Window::Create(sfg::Window::BACKGROUND);
			window->SetRequisition({ 35, 35 });
			m_itemTable->Attach(window, { i, j, 1, 1 });
		}
	}
}

sfg::Widget::Ptr PlayerInventoryWidget::GetWidget()
{ 
	return m_window; 
}