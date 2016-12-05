// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/Health.h>
#include <Entity.h>

Health::Health(EntityID ent)
	: Component(ent),
	  m_maxHealth(100.f),
	  m_health(100.f)
{
}

float Health::GetHealth() const
{
	return m_health;
}

float Health::GetHealthNormalized() const
{
	return m_health / m_maxHealth;
}

void Health::SetHealth(float hp)
{
	m_health = std::max(0.f, hp);
	if (m_health <= 0.f)
		entity->Destroy();
}

void Health::RemoveHealth(float amt)
{
	SetHealth(m_health - amt);
}
