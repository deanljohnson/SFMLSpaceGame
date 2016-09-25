#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

inline float Length(const sf::Vector2f& a)
{
	return sqrt(pow(a.x, 2) + pow(a.y, 2));
}

inline sf::Vector2f Normalize(const sf::Vector2f& a)
{
	auto l = Length(a);
	return (l != 0) 
		? a / l 
		: sf::Vector2f(); //length is zero, return zero vector
}