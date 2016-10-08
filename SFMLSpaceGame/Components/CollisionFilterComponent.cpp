#include <Components/CollisionFilterComponent.h>
#include <Entity.h>

bool CollisionFilterComponent::ShouldCollide(Entity* other) const
{
	return m_noCollision->GetID() != other->GetID();
}