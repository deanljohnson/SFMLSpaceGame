#pragma once
#include "Component.h"

class Position : Component
{
public:
	sf::Vector2f position;

	Position() : position(sf::Vector2f(0, 0)){}
	Position(const sf::Vector2f& p) : position(sf::Vector2f(p.x, p.y)){}

	float X() const noexcept { return position.x; }
	float Y() const noexcept { return position.y; }
};