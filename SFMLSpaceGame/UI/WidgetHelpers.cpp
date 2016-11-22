#include "stdafx.h"
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

	bool ValidateFloatEntry(sfg::Entry::Ptr entry)
	{
		std::string text = entry->GetText();

		// if text is empty, it's automatically invalid
		if (text.empty())
		{
			return false;
		}

		try
		{
			char* error = nullptr;
			auto val = strtof(text.c_str(), &error);
			if (error[0] != '\0')
			{
				return false;
			}
			return true;
		}
		catch (std::invalid_argument)
		{
			return false;
		}
	}
}