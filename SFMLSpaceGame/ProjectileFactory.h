#pragma once
#include <EntityID.h>
#include <memory>
#include <Box2D/Common/b2Math.h>

class ProjectileStats;

namespace ProjectileFactory
{
	EntityID CreateProjectile(std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
}