#pragma once
#include <ShipResourceSelector.h>
#include <SpawnLocationSelector.h>
#include <Event.h>
#include "Position.h"

class ShipSpawner : public Component
{
private:
	enum class SpawnType { Timed, Event } m_type;

	float m_time;
	float m_counter;

	EventType m_eventType;

	ShipResourceSelector m_shipSelector;
	SpawnLocationSelector m_locSelector;

	Position* m_position{ nullptr };

	bool m_spawningPlayer;
	
	void Spawn();

	void DoTimed();
	void DoEvent();

public:
	ShipSpawner(float time, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector)
		: m_type(SpawnType::Timed), m_time(time), m_counter(0.f), 
		  m_shipSelector(shipSelector), m_locSelector(locSelector),
		  m_spawningPlayer(false)
	{}
	ShipSpawner(EventType eventType, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector, bool spawnPlayer = false)
		: m_type(SpawnType::Event), m_eventType(eventType), m_shipSelector(shipSelector), 
		  m_locSelector(locSelector), m_spawningPlayer(spawnPlayer)
	{}

	virtual void Init() override;
	virtual void Update() override;
};