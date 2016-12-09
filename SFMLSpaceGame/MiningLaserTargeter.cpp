// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <MiningLaserTargeter.h>
#include <Box2D\Common\b2Math.h>
#include <EntityManager.h>
#include <EntityGroups.h>
#include <EntityHelpers.h>
#include <Entity.h>
#include <Components/Position.h>

MiningLaserTargeter::MiningLaserTargeter(float detectionRange)
	: m_detectionRange(detectionRange)
{
}

b2Vec2 MiningLaserTargeter::FindTarget(const b2Vec2& pos, EntityID source) const
{
	auto& asteroids = EntityManager::GetEntitiesByGroup(ASTEROID_GROUP);

	auto closestAsteroid = EntityHelpers::GetClosestEntity(pos, asteroids);

	if (closestAsteroid == nullptr)
		return pos;

	auto& asteroidPos = closestAsteroid->GetComponent<Position>();

	if (b2Distance(pos, asteroidPos.position) > m_detectionRange)
		return pos;

	return asteroidPos.position;
}

b2Vec2 MiningLaserTargeter::FindTarget(const b2Vec2& pos, EntityID source, EntityID& targetID) const
{
	// If we do not find an find an asteroid,
	// set target to null to tell callers
	// that we didn't find a target
	targetID = ENTITY_ID_NULL;

	auto& asteroids = EntityManager::GetEntitiesByGroup(ASTEROID_GROUP);

	auto closestAsteroid = EntityHelpers::GetClosestEntity(pos, asteroids);

	if (closestAsteroid == nullptr)
		return pos;

	auto& asteroidPos = closestAsteroid->GetComponent<Position>();

	if (b2Distance(pos, asteroidPos.position) > m_detectionRange)
		return pos;

	targetID = closestAsteroid->GetID();
	return asteroidPos.position;
}