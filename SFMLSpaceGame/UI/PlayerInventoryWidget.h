#pragma once
#include <SFGUI\Window.hpp>
#include <SFGUI\Table.hpp>

class PlayerInventoryWidget
{
private:
	sfg::Window::Ptr m_window;
	sfg::Table::Ptr m_itemTable;
public:
	PlayerInventoryWidget();

	void SetSize(const sf::Vector2i& size);

	sfg::Widget::Ptr GetWidget();
};