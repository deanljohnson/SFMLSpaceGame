#pragma once
#include "Component.h"
#include <EntityHandle.h>

// Prevents collisions between this entity and one other
class CollisionFilterComponent : public Component
{
private:
	EntityHandle& m_noCollision;

public:
	explicit CollisionFilterComponent(EntityHandle& noCollision)
		: m_noCollision(noCollision)
	{}

	bool ShouldCollide(Entity* other) const;
};