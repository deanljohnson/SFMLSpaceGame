#include <UI/UILayoutOption.h>
#include "UI.h"

void UILayoutOption::Init()
{
	m_elementIDs.clear();
}

void UILayoutOption::Apply()
{
	if (applied) return;

	applied = true;
	if (type == HorizontalGroup)
		ApplyHorizontalGrouping();
	else if (type == VerticalGroup)
		ApplyVerticalGrouping();
}

void UILayoutOption::Add(UI_ID id)
{
	m_elementIDs.push_back(id);
}

void UILayoutOption::ApplyHorizontalGrouping()
{
	float xLocation = 0.f;
	for (auto id : m_elementIDs)
	{
		auto elem = UI::Get(id);
		auto oldPos = elem->getPosition();
		elem->setPosition(oldPos + sf::Vector2f(xLocation, 0));

		xLocation += elem->GetBounds().width + oldPos.x;
	}
}

void UILayoutOption::ApplyVerticalGrouping()
{
	float yLocation = 0.f;
	for (auto id : m_elementIDs)
	{
		auto elem = UI::Get(id);
		auto oldPos = elem->getPosition();
		elem->setPosition(oldPos + sf::Vector2f(0, yLocation));

		yLocation += elem->GetBounds().height + oldPos.y;
	}
}
