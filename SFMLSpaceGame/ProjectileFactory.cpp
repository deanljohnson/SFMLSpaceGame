// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <ProjectileFactory.h>
#include <EntityManager.h>
#include <EntityGroups.h>
#include <EntityHandle.h>
#include <Entity.h>

#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/BulletPhysics.h>
#include <Components/CollisionFilterComponent.h>
#include <Components/RectPrimitive.h>
#include <Components/Lifetime.h>

namespace ProjectileFactory
{
	const size_t BULLET_POOL_SIZE = 100;
	std::vector<EntityID> POOLED_BULLETS;
}

void RemakeBullet(const EntityHandle& ent, std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	ent->GetComponent<Position>().position = p;
	ent->GetComponent<Rotation>().SetRadians(radians);

	auto& bulletPhys = ent->GetComponent<BulletPhysics>();
	bulletPhys.SetStats(proj);
	bulletPhys.SetSourceEntity(sourceEntity);
	bulletPhys.SetPosition(p);
	bulletPhys.SetAngle(radians);

	ent->GetComponent<CollisionFilterComponent>().SetNoCollisionEntity(sourceEntity);
	ent->GetComponent<RectPrimitive>().SetSize(proj->size.x, proj->size.y);
	ent->GetComponent<Lifetime>().Reset();
}

EntityID CreateNewProjectile(std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(PROJECTILE_GROUP);
	
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<BulletPhysics, EntityID, std::shared_ptr<ProjectileStats>>(sourceEntity, proj);
	ent->AddComponent<CollisionFilterComponent, EntityID>(sourceEntity);
	ent->AddComponent<RectPrimitive, float, float>(proj->size.x, proj->size.y);
	ent->AddComponent<Lifetime, float, bool>(proj->lifeTime, false);

	return ent.GetID();
}

EntityID ProjectileFactory::CreateProjectile(std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	for (size_t i = 0; i < POOLED_BULLETS.size(); i++)
	{
		if (!EntityManager::IsValidID(POOLED_BULLETS[i]))
		{
			POOLED_BULLETS.erase(POOLED_BULLETS.begin() + i);
			i--;
			continue;
		}

		EntityHandle handle = EntityManager::Get(POOLED_BULLETS[i]);
		if (!handle->isActive() && handle->isAlive())
		{
			handle->SetActive(true);
			RemakeBullet(handle, proj, sourceEntity, p, radians);
			return POOLED_BULLETS[i];
		}
	}

	EntityID newProjectile = CreateNewProjectile(proj, sourceEntity, p, radians);

	if (POOLED_BULLETS.size() < BULLET_POOL_SIZE)
	{
		POOLED_BULLETS.push_back(newProjectile);
	}
	
	return newProjectile;
}