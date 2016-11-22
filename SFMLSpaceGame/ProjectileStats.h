#pragma once
#include <Box2d\Common\b2Math.h>

class ProjectileStats
{
public:
	float speed{0.f};
	float lifeTime{0.f};
	float damage{0.f};
	b2Vec2 size{0.f, 0.f};
};