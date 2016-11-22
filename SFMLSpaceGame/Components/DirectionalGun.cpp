#include "stdafx.h"
#include <GameTime.h>
#include <Components/DirectionalGun.h>
#include <EntityFactory.h>
#include <VectorMath.h>
#include <WorldConstants.h>

DirectionalGun::DirectionalGun(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sprite(entity->GetComponent<Sprite>()),
	  m_gunData(nullptr)
{
}

void DirectionalGun::Update() 
{
	for (size_t i = 0; i < m_weaponStates.size(); i++) 
	{
		m_weaponStates[i].currentHeat -= m_gunData->rigs[i]->cooldownRate * GameTime::deltaTime;
		if (m_weaponStates[i].currentHeat < 0.f)
			m_weaponStates[i].currentHeat = 0.f;
	}
}

void DirectionalGun::FireWeapon(int i) 
{
	// fire a bullet for each hardpoint
	b2Rot rot(m_rotation.GetRadians());
	auto hp = m_gunData->hardPoints[i];

	// hard point offset is stored in pixel coordinates irrespective of the origin, must convert
	auto offset = (hp.positionOffset * METERS_PER_PIXEL) - m_sprite.GetOrigin();

	EntityFactory::CreateProjectile(m_gunData->rigs[i]->projectile, entity->GetID(),
		m_position.position + Rotate(offset, rot),
		m_rotation.GetRadians() + hp.angleOffset);

	// play a sound if available
	if (m_shotSound != nullptr)
		m_shotSound->Play();

	// Guns heat up each time they shoot
	m_weaponStates[i].currentHeat += m_gunData->rigs[i]->heatGenerated;

	// Store this so we can have a cooldown
	m_weaponStates[i].lastFiringTime = GameTime::totalTime;
}

void DirectionalGun::Shoot(const b2Vec2& pos)
{
	for (size_t i = 0; i < m_weaponStates.size(); i++) 
	{
		if (m_gunData->rigs[i] == nullptr)
			continue;

		float firingTime = m_weaponStates[i].lastFiringTime;
		float fireRate = m_gunData->rigs[i]->fireRate;
		
		float curHeat = m_weaponStates[i].currentHeat;
		float heatLimit = m_gunData->rigs[i]->heatLimit;

		// If we are on cooldown
		if ((GameTime::totalTime - firingTime) < fireRate
			|| curHeat > heatLimit)
		{
			continue;
		}

		FireWeapon(i);
	}
}

float DirectionalGun::GetNormalizedHeat() 
{
	// TODO: For now simply return the 0'th heat.
	// preferrably, this needs to be used in a way
	// that gives a better idea of the heat of the whole weapon system
	return m_weaponStates[0].currentHeat / m_gunData->rigs[0]->heatLimit;
}

void DirectionalGun::SetSoundSource(SoundSource* source)
{
	m_shotSound = source;
}

void DirectionalGun::SetGunData(DirectionalGunData* data)
{
	m_gunData = data;

	// will default construct the weapon stats
	// which is perfectly fine
	m_weaponStates.resize(data->rigs.size());
}