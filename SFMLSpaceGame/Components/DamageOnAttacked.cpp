// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/DamageOnAttacked.h>
#include <Components/Health.h>
#include <Interfaces/AttackEventModifier.h>
#include <Entity.h>

DamageOnAttacked::DamageOnAttacked(EntityID ent)
	: Component(ent),
	  m_health(entity->GetComponent<Health>()),
	  m_modifiers()
{
}

DamageOnAttacked::DamageOnAttacked(EntityID ent, std::initializer_list<AttackedEventModifier*> modifiers)
	: Component(ent),
	  m_health(entity->GetComponent<Health>()),
	  m_modifiers(modifiers)
{
}

void DamageOnAttacked::Update()
{
	if (entity->events.Count() > 0)
	{
		AttackedEvent* attackdEvent{ nullptr };
		if (attackdEvent = entity->events.Get<EventType::Attacked>())
		{
			HandleAttack(attackdEvent);
		}
	}
}

void DamageOnAttacked::AddModifier(AttackedEventModifier* modifier)
{
	m_modifiers.push_back(modifier);
}

void DamageOnAttacked::HandleAttack(AttackedEvent* event) 
{
	for (auto m : m_modifiers) 
	{
		m->Modify(event);
	}

	m_health.RemoveHealth(event->damage);
}
