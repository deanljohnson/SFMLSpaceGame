#pragma once
#include "Component.h"

// Prevents collisions between this entity and one other
class CollisionFilterComponent : public Component
{
private:
	Entity* m_noCollision{nullptr};

public:
	explicit CollisionFilterComponent(Entity* noCollision)
		: m_noCollision(noCollision)
	{}

	bool ShouldCollide(Entity* other) const;
};