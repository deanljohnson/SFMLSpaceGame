#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <ExtendedMath.h>

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

/// Rotate a vector
inline sf::Vector2f Rotate(const sf::Vector2f& v, const b2Rot& q)
{
	return sf::Vector2f(q.c * v.x - q.s * v.y, q.s * v.x + q.c * v.y);
}

inline b2Vec2 Rotate(const b2Vec2& v, float r)
{
	auto ca = cos(r);
	auto sa = sin(r);
	return b2Vec2(ca*v.x - sa*v.y, sa*v.x + ca*v.y);
}

inline sf::Vector2f Rotate(const sf::Vector2f& v, float r)
{
	auto ca = cos(r);
	auto sa = sin(r);
	return sf::Vector2f(ca*v.x - sa*v.y, sa*v.x + ca*v.y);
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

inline sf::Vector2f AtAngleDegrees(float deg)
{
	auto rad = deg * static_cast<float>(M_PI) / 180.f;
	return sf::Vector2f(cosf(rad), sinf(rad));
}

inline sf::Vector2f AtAngleRad(float rad)
{
	return sf::Vector2f(cosf(rad), sinf(rad));
}

// Takes a line A defined by a start point a1 and an end point a2
// and returns whether a point b is on the right of that line or not
inline bool IsRight(b2Vec2 a1, b2Vec2 a2, b2Vec2 b)
{
	return ((a2.x - a1.x)*(b.y - a1.y) - (a2.y - a1.y)*(b.x - a1.x)) > 0;
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