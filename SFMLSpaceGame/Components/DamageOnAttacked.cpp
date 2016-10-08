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
		if (entity->events.Get(Attacked, event))
		{
			m_health->RemoveHealth(event.attacked.damage);
		}
	}
}
