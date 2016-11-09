#include "stdafx.h"
#include <Components/Lifetime.h>
#include <GameTime.h>
#include <Entity.h>

Lifetime::Lifetime(EntityID ent, float lifetime)
	: Component(ent),
	  m_lifetime(lifetime)
{}

void Lifetime::Update()
{
	m_lifetime -= GameTime::deltaTime;

	if (m_lifetime <= 0)
		entity->Destroy();
}