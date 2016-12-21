// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/WidgetHelpers.h>
#include <GameView.h>

namespace WidgetHelpers
{
	void CenterAt(std::shared_ptr<sfg::Widget> widget, const sf::Vector2f& pos)
	{
		auto alloc = widget->GetAllocation();
		widget->SetPosition({pos.x - (alloc.width / 2.f),
							pos.y - (alloc.height / 2.f)});
	}

	sf::Vector2f GetRelativeMousePos(std::shared_ptr<sfg::Widget> widget)
	{
		auto mousePos = GameView::GetScreenMouseLocation();
		auto canvasPos = widget->GetAbsolutePosition();
		return mousePos - canvasPos;
	}

	bool MouseInWidget(std::shared_ptr<sfg::Widget> widget) 
	{
		auto relPos = GetRelativeMousePos(widget);

		return relPos.x > 0 && relPos.x < widget->GetAllocation().width
			&& relPos.y > 0 && relPos.y < widget->GetAllocation().height;
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