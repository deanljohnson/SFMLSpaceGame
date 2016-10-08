#include <Components/ShipAI.h>
#include <EntityManager.h>
#include <Event.h>

void ShipAI::Init()
{
	m_controller = &entity->GetComponent<ShipController>();
}

void ShipAI::Update()
{
	ProcessEvents();
	ProcessAIState();
}

void ShipAI::ProcessEvents()
{
	if (entity->events.Count() > 0)
	{
		auto it = entity->events.Begin();
		while (it != entity->events.End())
		{
			if (it->type == Attacked)
			{
				HandleAttackedEvent(it->attacked);
			}
			++it;
		}
	}
}

void ShipAI::ProcessAIState()
{
	switch(m_currentState)
	{
	case AIState::None:
		break;
	case AIState::AttackingShip:
		if (!m_targetHandle.IsValid())
		{
			m_controller->Clear();
			m_currentState = AIState::None;
		}			
		break;
	}
}

void ShipAI::HandleAttackedEvent(Event::AttackedEvent event)
{
	auto& ent = entity->GetManager()->Get(event.attackerID);

	// Make sure the handle is still valid
	if (!ent.IsValid()) return;

	m_targetHandle = ent;

	m_controller->SetTarget(ent);
	m_controller->Set(StrafeToTargetsRearForAttack);
	m_controller->Set(FireGunsWhenFacingTarget);

	m_currentState = AIState::AttackingShip;
}
