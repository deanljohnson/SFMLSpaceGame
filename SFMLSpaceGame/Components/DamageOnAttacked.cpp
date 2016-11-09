#include "stdafx.h"
#include <Components/DamageOnAttacked.h>
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
		Event event;
		if (entity->events.Get(EventType::Attacked, event))
		{
			HandleAttack(event);
		}
	}
}

void DamageOnAttacked::HandleAttack(Event& event) 
{
	for (auto m : m_modifiers) 
	{
		m->Modify(event);
	}

	m_health.RemoveHealth(event.attacked.damage);
}
