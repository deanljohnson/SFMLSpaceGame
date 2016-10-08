#include <Components/CollisionFilterComponent.h>
#include <Entity.h>
#include <EntityManager.h>

void CollisionFilterComponent::Init()
{
	m_noCollision = entity->GetManager()->Get(m_noCollisionID);
}

bool CollisionFilterComponent::ShouldCollide(Entity* other)
{
	if (!m_noCollision.IsValid()) return false;
	return m_noCollision->GetID() != other->GetID();
}