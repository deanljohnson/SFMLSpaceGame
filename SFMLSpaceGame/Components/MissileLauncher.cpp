#include "stdafx.h"
#include <Components\MissileLauncher.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <VectorMath.h>

MissileLauncher::MissileLauncher(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sprite(entity->GetComponent<Sprite>()),
	  m_lastFiringTime(0),
	  m_launcherData(nullptr)
{
}

void MissileLauncher::Shoot()
{
	// If we are on cooldown
	if ((GameTime::totalTime - m_lastFiringTime) < m_launcherData->fireRate)
	{
		return;
	}

	// fire a missile for each hardpoint
	b2Rot rot(m_rotation.GetRadians());
	for (auto hp : m_launcherData->hardPoints)
	{
		// TODO: for now we are simply using a bullet. Need to change to be a missile
		// hard point offset is stored in pixel coordinates irrespective of the origin, must convert
		auto offset = (hp.positionOffset * METERS_PER_PIXEL) - m_sprite.GetOrigin();
		EntityFactory::CreateMissile("MissileOne", entity->GetID(),
			m_position.position + Rotate(offset, rot),
			m_rotation.GetRadians() + hp.angleOffset);
	}

	// play a sound if available
	if (m_shotSound != nullptr)
		m_shotSound->Play();

	// Store this so we can have a cooldown
	m_lastFiringTime = GameTime::totalTime;
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
}