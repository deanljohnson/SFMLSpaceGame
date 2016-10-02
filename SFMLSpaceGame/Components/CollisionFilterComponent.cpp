#include <Components/CollisionFilterComponent.h>

bool CollisionFilterComponent::ShouldCollide(Entity* other) const
{
	return m_noCollision != other;
}