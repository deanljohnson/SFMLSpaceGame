#include <Components/ShipAI.h>
#include <EntityManager.h>
#include <EntityGroups.h>
#include <Event.h>
#include <EntityHelpers.h>

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
		Event event;
		if (entity->events.Get(Attacked, event))
		{
			HandleAttackedEvent(event.attacked);
		}
	}
}

void ShipAI::ProcessAIState()
{
	switch(m_currentState)
	{
	case AIState::None:
		FindStation();
		break;
	case AIState::AttackingShip:
		// target is dead or otherwise "gone"
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
	auto ent = EntityManager::Get(event.attackerID);

	// Make sure the handle is still valid
	if (!ent.IsValid()) return;

	m_targetHandle = ent;

	m_controller->SetTarget(event.attackerID);
	m_controller->Set(StrafeToTargetsRearForAttack);
	m_controller->Set(FireGunsWhenFacingTarget);

	m_currentState = AIState::AttackingShip;
}

void ShipAI::FindStation()
{
	auto& stations = EntityManager::GetEntitiesByGroup(STATION_GROUP);
	Entity* closest = EntityHelpers::GetClosestEntity(entity, stations);

	if (closest == nullptr) return;

	m_targetHandle = EntityManager::Get(closest->GetID());
	m_controller->SetTarget(m_targetHandle->GetID());
	m_controller->Set(Maneuvers::Approach);
}

