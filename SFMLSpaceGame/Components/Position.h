#pragma once
#include <Components/Component.h>

class Position : public Component
{
public:
	sf::Vector2f position;

	Position() : position(sf::Vector2f(0, 0)){}
	explicit Position(const sf::Vector2f& p) : position(p){}

	float X() const noexcept { return position.x; }
	float Y() const noexcept { return position.y; }
};