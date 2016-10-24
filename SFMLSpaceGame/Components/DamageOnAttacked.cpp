#include <Components/DamageOnAttacked.h>
#include <Entity.h>

void DamageOnAttacked::Init()
{
	m_health = &entity->GetComponent<Health>();
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

	m_health->RemoveHealth(event.attacked.damage);
}
