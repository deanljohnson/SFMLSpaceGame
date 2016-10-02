#pragma once
#include "Component.h"

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