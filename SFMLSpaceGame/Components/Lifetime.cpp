// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/Lifetime.h>
#include <GameTime.h>
#include <Entity.h>

Lifetime::Lifetime(EntityID ent, float lifetime, bool destroyOnExpire)
	: Component(ent),
	m_maxLifetime(lifetime),
	m_lifetime(lifetime),
	m_destroy(destroyOnExpire)
{}

void Lifetime::Update()
{
	m_lifetime -= GameTime::deltaTime;

	if (m_lifetime <= 0)
	{
		if (m_destroy)
			entity->Destroy();
		else
			entity->SetActive(false);
	}
}

void Lifetime::Reset()
{
	m_lifetime = m_maxLifetime;
}