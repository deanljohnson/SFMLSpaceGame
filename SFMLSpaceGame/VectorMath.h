#pragma once
#include <Box2D/Dynamics/b2Body.h>
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

inline sf::Vector2f B2VecToSFMLVec(const b2Vec2& v) 
{
	return sf::Vector2f(v.x, v.y);
}

inline b2Vec2 SFMLVecToB2Vec(const sf::Vector2f& v) 
{
	return b2Vec2(v.x, v.y);
}

inline b2Vec2 Rotate(const b2Vec2& v, float r)
{
	auto ca = cosf(r);
	auto sa = cosf(r);
	return b2Vec2(ca*v.x - sa*v.y, sa*v.x + ca*v.y);
}