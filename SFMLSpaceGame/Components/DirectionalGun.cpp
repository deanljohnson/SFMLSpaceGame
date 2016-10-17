#include <GameTime.h>
#include <Components/DirectionalGun.h>
#include <EntityManager.h>
#include <EntityFactory.h>
#include <VectorMath.h>
#include <resource.h>
#include <WorldConstants.h>

void DirectionalGun::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
	m_sprite = &entity->GetComponent<Sprite>();
}

void DirectionalGun::Update() 
{
	m_currentHeat -= m_gunData->cooldownRate * GameTime::deltaTime;
	if (m_currentHeat < 0) m_currentHeat = 0.f;
}

void DirectionalGun::Shoot()
{
	// If we are on cooldown
	if ((GameTime::totalTime - m_lastFiringTime) < m_gunData->fireRate
		|| m_currentHeat > m_gunData->heatLimit)
	{
		return;
	}

	// fire a bullet for each hardpoint
	b2Rot rot(m_rotation->GetRadians());
	for (auto hp : m_gunData->hardPoints)
	{
		// hard point offset is stored in pixel coordinates irrespective of the origin, must convert
		auto offset = (hp.positionOffset * METERS_PER_PIXEL) - m_sprite->GetOrigin();
		EntityFactory::CreateProjectile(PROJECTILE_LASER_ONE, entity->GetID(), m_position->position + Rotate(offset, rot), m_rotation->GetRadians() + hp.angleOffset);
	}

	if (m_shotSound != nullptr)
		m_shotSound->Play();

	m_currentHeat += m_gunData->heatGenerated;

	// Store this so we can have a cooldown
	m_lastFiringTime = GameTime::totalTime;
}

float DirectionalGun::GetNormalizedHeat() 
{
	return m_currentHeat / m_gunData->heatLimit;
}

void DirectionalGun::SetSoundSource(SoundSource* source)
{
	m_shotSound = source;
}