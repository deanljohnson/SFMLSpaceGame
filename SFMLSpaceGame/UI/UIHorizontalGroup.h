#pragma once
#include <UI/UIElement.h>

class UIHorizontalGroup : public UIElement 
{
public:
	virtual sf::FloatRect GetBounds() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};