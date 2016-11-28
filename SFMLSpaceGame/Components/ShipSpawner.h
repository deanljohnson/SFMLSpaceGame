#pragma once
#include <ShipResourceSelector.h>
#include <SpawnLocationSelector.h>
#include <Event.h>

class Position;

class ShipSpawner : public Component
{
private:
	enum class SpawnType { Timed, Event } m_type;

	float m_time;
	float m_counter;

	EventType m_eventType;

	ShipResourceSelector m_shipSelector;
	SpawnLocationSelector m_locSelector;

	Position& m_position;

	bool m_spawningPlayer;
	
	void Spawn();

	void DoTimed();
	void DoEvent();

public:
	ShipSpawner(EntityID ent, float time, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector);
	ShipSpawner(EntityID ent, EventType eventType, const ShipResourceSelector& shipSelector, const SpawnLocationSelector& locSelector, bool spawnPlayer = false);

	virtual void Update() override;
};