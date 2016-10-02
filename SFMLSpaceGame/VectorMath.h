#pragma once
#include <Box2D/Common/b2Math.h>
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

inline b2Vec2 Rotate(const b2Vec2& v, const b2Rot& r)
{
	return b2Mul(r, v);
}

inline b2Vec2 Rotate(const b2Vec2& v, float r)
{
	auto ca = cos(r);
	auto sa = sin(r);
	return b2Vec2(ca*v.x - sa*v.y, sa*v.x + ca*v.y);
}

inline sf::Vector2f MultComponents(const sf::Vector2f& a, const sf::Vector2f& b)
{
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}

inline sf::Vector2f Lerp(const sf::Vector2f& a, const sf::Vector2f& b, float d)
{
	return a + ((b - a) * d);
}

inline b2Vec2 Lerp(const b2Vec2& a, const b2Vec2& b, float d)
{
	auto t = b - a;
	return a + (b2Vec2(t.x * d, t.y * d));
}

inline sf::Vector2f MultB2VecToSFMLVec(const b2Vec2& a, float b)
{
	return sf::Vector2f(a.x * b, a.y * b);
}

inline b2Vec2 operator *(const b2Vec2& a, float b)
{
	return b2Vec2(a.x * b, a.y * b);
}

inline b2Vec2 operator /(const b2Vec2& a, float b)
{
	return b2Vec2(a.x / b, a.y / b);
}