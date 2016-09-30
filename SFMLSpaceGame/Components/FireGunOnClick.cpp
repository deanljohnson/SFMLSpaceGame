#include <Entity.h>
#include <Components/FireGunOnClick.h>
#include <Components/DirectionalGun.h>
#include <Components/GameWorldClickListener.h>
#include <Interfaces/Gun.h>

void FireGunOnClick::Init() 
{
	if (entity->HasComponent<DirectionalGun>()) 
	{
		m_guns.push_back((Gun*)(&entity->GetComponent<DirectionalGun>()));
	}

	if (entity->HasComponent<GameWorldClickListener>()) 
	{
		auto gwcl = &entity->GetComponent<GameWorldClickListener>();
		gwcl->AddClickListener(this);
		gwcl->AddHeldListener(this);
	}
}

void FireGunOnClick::OnClick(b2Vec2 pos) 
{
}

void FireGunOnClick::OnHeld(b2Vec2 pos) 
{
	for (auto g : m_guns) 
	{
		g->Shoot();
	}
}