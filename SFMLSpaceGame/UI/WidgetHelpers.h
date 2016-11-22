#pragma once
#include <SFGUI/Widget.hpp>

namespace WidgetHelpers
{
	sf::Vector2f GetRelativeMousePos(std::shared_ptr<sfg::Widget> widget);
	bool ValideFloatEntry(sfg::Entry::Ptr entry);
}