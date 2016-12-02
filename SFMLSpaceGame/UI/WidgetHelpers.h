#pragma once
#include <SFGUI/Widget.hpp>

namespace WidgetHelpers
{
	void CenterAt(std::shared_ptr<sfg::Widget> widget, const sf::Vector2f& pos);
	sf::Vector2f GetRelativeMousePos(std::shared_ptr<sfg::Widget> widget);
	bool MouseInWidget(std::shared_ptr<sfg::Widget> widget);
	bool ValidateFloatEntry(sfg::Entry::Ptr entry);
}