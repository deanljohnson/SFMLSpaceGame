#include <Components/ShipSpawner.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <Entity.h>

void ShipSpawner::Init()
{
	m_position = &entity->GetComponent<Position>();
}

void ShipSpawner::Update()
{
	m_counter += GameTime::deltaTime;
	if (m_counter > m_time)
	{
		m_counter = 0.f;
		auto id = m_shipSelector.Select();
		auto loc = m_locSelector.Select(m_position->position);
		EntityFactory::CreateShip(id, loc);
	}
}