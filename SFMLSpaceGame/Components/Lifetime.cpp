// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
