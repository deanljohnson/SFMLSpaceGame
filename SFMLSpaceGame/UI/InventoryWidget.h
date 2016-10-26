#pragma once
#include <SFGUI\Window.hpp>
#include <SFGUI\Table.hpp>
#include <EntityID.h>
#include <EntityHandle.h>
#include <UI\InventoryItemWidget.h>
#include <SFGUI\Label.hpp>

class InventoryWidget
{
private:
	sfg::Window::Ptr m_window;
	sfg::Table::Ptr m_itemTable;
	sfg::Label::Ptr m_creditsLabel;

	EntityHandle m_targetHandle;

	std::vector<InventoryItemWidget::Ptr> m_itemWidgets;

	sf::Vector2i m_size;
public:
	explicit InventoryWidget(bool displayCredits);

	void Update();

	void SetSize(const sf::Vector2i& size);

	sfg::Widget::Ptr GetWidget();
	void SetTarget(EntityID id);
};