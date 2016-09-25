#pragma once
#include "Component.h"

const float PIXELS_PER_METER = 100;

class Position : public Component
{
public:
	sf::Vector2f position;

	Position() : position(sf::Vector2f(1, 1)){}
	explicit Position(const sf::Vector2f& p) : position(sf::Vector2f(p.x, p.y)){}

	float X() const noexcept { return position.x; }
	float Y() const noexcept { return position.y; }
};