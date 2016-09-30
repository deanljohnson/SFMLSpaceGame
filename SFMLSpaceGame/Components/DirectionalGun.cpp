#include <Components/DirectionalGun.h>
#include <Entity.h>

void DirectionalGun::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
}

void DirectionalGun::Shoot()
{
	printf("Shooting!\n");
}