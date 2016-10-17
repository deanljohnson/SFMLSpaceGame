#include <UI/WidgetHelpers.h>
#include <WorldConstants.h>

namespace WidgetHelpers
{
	sf::Vector2f GetRelativeMousePos(std::shared_ptr<sfg::Widget> widget)
	{
		auto mousePos = sf::Mouse::getPosition(*GAME_WINDOW);
		auto canvasPos = widget->GetAbsolutePosition();
		return sf::Vector2f(mousePos.x, mousePos.y) - canvasPos;
	}
}