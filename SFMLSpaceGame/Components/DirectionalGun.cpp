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

	auto bullet = entity->GetManager().AddEntity(entity->GetWorld(), Group(2));
	EntityFactory::MakeIntoBullet(bullet, m_position->position, m_rotation->GetRadians());

	m_lastFiringTime = GameTime::totalTime;
}