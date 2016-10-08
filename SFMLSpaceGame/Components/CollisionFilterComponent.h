#pragma once
#include "Component.h"
#include <EntityHandle.h>

// Prevents collisions between this entity and one other
class CollisionFilterComponent : public Component
{
private:
	EntityHandle m_noCollision;
	EntityID m_noCollisionID;

public:
	explicit CollisionFilterComponent(EntityID noCollision)
		: m_noCollisionID(noCollision)
	{}
	virtual void Init() override;
	bool ShouldCollide(Entity* other);
};