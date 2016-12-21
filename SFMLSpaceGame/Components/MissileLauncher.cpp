// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components\MissileLauncher.h>
#include <Components\Position.h>
#include <Components\Rotation.h>
#include <Components\Sprite.h>
#include <Components\SoundSource.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <EntityManager.h>
#include <EntityGroups.h>
#include <EntityHelpers.h>
#include <VectorMath.h>
#include <Entity.h>
#include <GameView.h>

MissileLauncher::MissileLauncher(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sprite(entity->GetComponent<Sprite>()),
	  m_launcherData(nullptr)
{
}

EntityID MissileLauncher::GetTarget(const b2Vec2& firingPoint) 
{
	auto& ships = EntityManager::GetEntitiesByGroup(entity->HasGroup(PLAYER_GROUP) 
															? NON_PLAYER_SHIP_GROUP 
															: PLAYER_GROUP);

	Entity* ship = EntityHelpers::GetClosestEntity(firingPoint, 
													ships, 
													[this](Entity* e) { return e->GetID() != entity->GetID(); });

	if (ship == nullptr)
		return ENTITY_ID_NULL;

	return ship->GetID();
}

void MissileLauncher::FireWeapon(int i, EntityID target)
{
	// fire a missile for each hardpoint
	b2Rot rot(m_rotation.GetRadians());
	auto hp = m_launcherData->hardPoints[i];

	// hard point offset is stored in pixel coordinates irrespective of the origin, must convert
	auto offset = (hp.positionOffset * GameView::METERS_PER_PIXEL) - m_sprite.GetOrigin();

	EntityFactory::CreateMissile(m_launcherData->rigs[i]->missile, entity->GetID(), target,
		m_position.position + Rotate(offset, rot),
		m_rotation.GetRadians() + hp.angleOffset);

	// play a sound if available
	if (m_shotSound != nullptr)
		m_shotSound->Play();

	m_lastFiringTimes[i] = GameTime::totalTime;
}

void MissileLauncher::Shoot(const b2Vec2& pos)
{
	EntityID target = 0;
	bool targetSet = false;

	for (size_t i = 0; i < m_lastFiringTimes.size(); i++)
	{
		if (m_launcherData->rigs[i] == nullptr)
			continue;

		float fireRate = m_launcherData->rigs[i]->fireRate;

		// If we are on cooldown
		if ((GameTime::totalTime - m_lastFiringTimes[i]) < fireRate)
			continue;

		// We only want to set the target once,
		// but want to delay it as long as possible
		// in case we aren't going to fire in this 
		// call because of cooldowns
		if (!targetSet)
		{
			target = GetTarget(pos);
			targetSet = true;
		}

		FireWeapon(i, target);
	}
}

float MissileLauncher::GetNormalizedHeat()
{
	return 0.f;
}

void MissileLauncher::SetSoundSource(SoundSource* source)
{
	m_shotSound = source;
}

void MissileLauncher::SetLauncherData(MissileLauncherData* data)
{
	m_launcherData = data;

	m_lastFiringTimes.resize(data->rigs.size(), 0.f);
}

void MissileLauncherData::LoadNewMissileRig(const std::string& rigName, size_t index)
{
	assert(index < rigs.size());

	rigs[index] = LoadRig<MissileRig>(rigName);
}