#include <Components/CollisionFilterComponent.h>

bool CollisionFilterComponent::ShouldCollide(Entity* other)
{
	return m_noCollision != other;
}