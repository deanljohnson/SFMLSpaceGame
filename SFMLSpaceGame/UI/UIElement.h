#pragma once
#include <SFML/Graphics/Transform.hpp>

class UI;

namespace sf{
	class RenderStates;
	class RenderTarget;
}

class UIElement
{
	friend UI;
protected:
	sf::Transform m_transform;
public:
	virtual ~UIElement()
	{
	}

	virtual void Refresh() {}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) = 0;
};