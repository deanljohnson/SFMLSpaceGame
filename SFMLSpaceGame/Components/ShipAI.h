#pragma once
#include "Component.h"
#include "ShipController.h"
#include <Event.h>

class ShipAI : public Component
{
private:
	enum class AIState{ None, AttackingShip, MovingToStation } m_currentState { AIState::None };

	EntityID m_lastStationReached{ ENTITY_ID_NULL };
	Position* m_stationPosition{ nullptr };

	std::shared_ptr<ShipStats> m_shipStats;
	ShipController* m_controller{ nullptr };
	Position* m_position{ nullptr };
	EntityHandle m_targetHandle;

	void ProcessEvents();
	void ProcessAIState();
	void HandleAttackedEvent(Event::AttackedEvent event);

	void FindStation();
public:
	explicit ShipAI(std::shared_ptr<ShipStats> stats)
		: m_shipStats(stats)
	{}
	virtual void Init() override;
	virtual void Update() override;
};