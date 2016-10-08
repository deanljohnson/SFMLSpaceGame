#pragma once
#include "Component.h"
#include <ShipResourceSelector.h>
#include <SpawnLocationSelector.h>

class ShipSpawner : public Component
{
private:
	float m_time;
	float m_counter;
	ShipResourceSelector m_shipSelector;
	SpawnLocationSelector m_locSelector;

	Position* m_position{ nullptr };
	
public:
	ShipSpawner(float time, ShipResourceSelector shipSelector, SpawnLocationSelector locSelector)
		: m_time(time), m_counter(0.f), m_shipSelector(shipSelector), m_locSelector(locSelector)
	{}

	virtual void Init() override;
	virtual void Update() override;
};