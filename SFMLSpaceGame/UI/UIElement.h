#pragma once

namespace sf{
	class RenderStates;
	class RenderTarget;
}

class UIElement
{
public:
	virtual ~UIElement()
	{
	}

	virtual void Refresh() {}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates& states) = 0;
};