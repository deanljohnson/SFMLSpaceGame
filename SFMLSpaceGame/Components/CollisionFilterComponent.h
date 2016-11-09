#pragma once
#include "Component.h"
#include <EntityHandle.h>

// Prevents collisions between this entity and one other
class CollisionFilterComponent : public Component
{
private:
	EntityHandle m_noCollision;

public:
	CollisionFilterComponent(EntityID ent, EntityID noCollision);
	bool ShouldCollide(Entity* other);
};