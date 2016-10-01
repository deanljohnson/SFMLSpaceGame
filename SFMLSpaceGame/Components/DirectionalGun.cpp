#include <GameTime.h>
#include <Components/DirectionalGun.h>
#include <Entity.h>
#include <EntityManager.h>
#include <EntityFactory.h>

void DirectionalGun::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
}

void DirectionalGun::Shoot()
{
	if ((GameTime::totalTime - m_lastFiringTime) < m_cooldown)
		return;

	b2Rot rot(m_rotation->GetRadians());
	for (auto hp : m_hardPoints)
	{
		auto bullet = entity->GetManager().AddEntity(entity->GetWorld(), Group(2));
		EntityFactory::MakeIntoBullet(bullet, m_position->position + b2Mul(rot, hp.positionOffset), m_rotation->GetRadians() + hp.angleOffset);
	}

	m_lastFiringTime = GameTime::totalTime;
}