#pragma once
#include <Components/Component.h>
#include <Box2D/Common/b2Math.h>

class Position : public Component
{
public:
	b2Vec2 position;

	explicit Position(EntityID ent) : Component(ent), position(b2Vec2(0, 0)){}
	Position(EntityID ent, const b2Vec2& p) : Component(ent), position(p){}

	float X() const noexcept { return position.x; }
	float Y() const noexcept { return position.y; }

	b2Vec2 operator+(const Position& other) const { return position + other.position; }
	b2Vec2 operator-(const Position& other) const { return position - other.position; }
};