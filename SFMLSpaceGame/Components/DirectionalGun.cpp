#include <GameTime.h>
#include <Components/DirectionalGun.h>
#include <Entity.h>
#include <EntityManager.h>
#include <EntityFactory.h>
#include <VectorMath.h>
#include <EntityGroups.h>

void DirectionalGun::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
}

void DirectionalGun::Shoot()
{
	// If we are on cooldown
	if ((GameTime::totalTime - m_lastFiringTime) < m_cooldown)
		return;

	// fire a bullet for each hardpoint
	b2Rot rot(m_rotation->GetRadians());
	for (auto hp : m_hardPoints)
	{
		auto bullet = entity->GetManager()->AddEntity(entity->GetWorld(), PROJECTILE_GROUP);
		EntityFactory::MakeIntoBullet(bullet, entity, m_position->position + Rotate(hp.positionOffset, rot), m_rotation->GetRadians() + hp.angleOffset);
	}

	// Store this so we can have a cooldown
	m_lastFiringTime = GameTime::totalTime;
}