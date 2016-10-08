#pragma once
#include "Component.h"
#include "ShipController.h"
#include <Event.h>

class ShipAI : public Component
{
private:
	enum class AIState{ None, AttackingShip } m_currentState { AIState::None };

	ShipController* m_controller{ nullptr };
	EntityHandle m_targetHandle;

	void ProcessEvents();
	void ProcessAIState();
	void HandleAttackedEvent(Event::AttackedEvent event);

	void FindStation();
public:
	ShipAI(){}
	virtual void Init() override;
	virtual void Update() override;
};