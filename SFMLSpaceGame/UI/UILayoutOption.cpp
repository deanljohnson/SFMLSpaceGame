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

	for (auto type : m_layoutOperations) 
	{
		if (type == HorizontalGroup)
			ApplyHorizontalGrouping();
		else if (type == VerticalGroup)
			ApplyVerticalGrouping();
		else if (type == CenterHorizontal)
			ApplyCenterHorizontal();
	}
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
		elem->setPosition(oldPos + sf::Vector2f(xLocation, 0) + position);

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
		elem->setPosition(oldPos + sf::Vector2f(0, yLocation) + position);

		yLocation += elem->GetBounds().height + oldPos.y;
	}
}

void UILayoutOption::ApplyCenterHorizontal()
{
	float maxRight = 0.f;
	for (auto id : m_elementIDs)
	{
		auto elem = UI::Get(id);
		maxRight = std::max(maxRight, elem->GetBounds().width + elem->getPosition().x);
	}

	sf::Vector2f center = sf::Vector2f(maxRight / 2.f, 0.f);
	for (auto id : m_elementIDs)
	{
		auto elem = UI::Get(id);
		auto oldPos = elem->getPosition();
		auto bounds = elem->GetBounds();
		auto horizDif = center.x - (bounds.left + bounds.width / 2.f);
		auto horizAdjustment = sf::Vector2f(horizDif, 0.f);
		elem->setPosition(oldPos + horizAdjustment + position);
	}
}
