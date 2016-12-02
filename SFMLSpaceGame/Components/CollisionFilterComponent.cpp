// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/CollisionFilterComponent.h>
#include <Entity.h>
#include <EntityManager.h>

CollisionFilterComponent::CollisionFilterComponent(EntityID ent, EntityID noCollision)
	: Component(ent)
{
	m_noCollision = EntityManager::Get(noCollision);
}

bool CollisionFilterComponent::ShouldCollide(Entity* other)
{
	if (!m_noCollision.IsValid()) return false;
	return m_noCollision->GetID() != other->GetID();
}