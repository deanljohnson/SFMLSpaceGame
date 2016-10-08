#pragma once
#include <Components/Component.h>
#include <Box2D/Common/b2Math.h>

class Position : public Component
{
public:
	b2Vec2 position;

	Position() : position(b2Vec2(0, 0)){}
	explicit Position(const b2Vec2& p) : position(p){}

	float X() const noexcept { return position.x; }
	float Y() const noexcept { return position.y; }

	b2Vec2 operator+(const Position& other) { return position + other.position; }
	b2Vec2 operator-(const Position& other) { return position - other.position; }
};