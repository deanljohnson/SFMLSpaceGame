#include "stdafx.h"
#include <Components/ShipSpawner.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <Entity.h>
#include <GameState.h>
#include <PlayerData.h>

ShipSpawner::ShipSpawner(EntityID ent, float time, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector) 
	: Component(ent), 
	  m_type(SpawnType::Timed), 
	  m_time(time), 
	  m_counter(0.f),
	  m_shipSelector(shipSelector), 
	  m_locSelector(locSelector),
	  m_position(entity->GetComponent<Position>()),
	  m_spawningPlayer(false)
{}

ShipSpawner::ShipSpawner(EntityID ent, EventType eventType, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector, bool spawnPlayer) 
	: Component(ent), 
	  m_type(SpawnType::Event), 
	  m_eventType(eventType), 
	  m_shipSelector(shipSelector),
	  m_locSelector(locSelector), 
	  m_position(entity->GetComponent<Position>()),
	  m_spawningPlayer(spawnPlayer)
{}

void ShipSpawner::Update()
{
	switch(m_type)
	{
	case SpawnType::Timed:
		DoTimed();
		break;
	case SpawnType::Event:
		DoEvent();
		break;
	}
	
}

void ShipSpawner::DoTimed()
{
	m_counter += GameTime::deltaTime;
	if (m_counter > m_time)
	{
		m_counter = 0.f;
		Spawn();
	}
}

void ShipSpawner::DoEvent()
{
	Event event;
	if (GameState::pendingGameEvents.Get(m_eventType, event))
	{
		Spawn();
	}
}

void ShipSpawner::Spawn()
{
	if (!m_spawningPlayer)
	{
		auto shipName = m_shipSelector.Select();
		auto loc = m_locSelector.Select(m_position.position);
		EntityFactory::CreateShip(shipName, loc);
	}
	else
	{
		auto loc = m_locSelector.Select(m_position.position);
		auto playerID = EntityFactory::CreatePlayer(loc);
		PlayerData::GetActive()->SetID(playerID);

		Event playerSpawned = Event();
		playerSpawned.playerSpawned = Event::PlayerSpawnedEvent();
		playerSpawned.playerSpawned.ID = playerID;
		playerSpawned.type = EventType::PlayerSpawned;

		GameState::pendingGameEvents.Push(playerSpawned);
	}
}
