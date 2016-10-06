#pragma once
#include "UIElement.h"

class UIArea : public UIElement
{
private:
	sf::FloatRect m_rect;
public:
	UIArea(float x, float y, float w, float h)
		: m_rect(x,y,w,h)
	{}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	virtual sf::FloatRect GetBounds() override;
};