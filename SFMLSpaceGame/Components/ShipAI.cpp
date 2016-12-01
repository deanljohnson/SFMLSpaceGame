#include "stdafx.h"
#include <Components/ShipAI.h>
#include <Components/Position.h>
#include <Components/ShipController.h>
#include <EntityManager.h>
#include <EntityGroups.h>
#include <Event.h>

namespace
{
	const float LOW_THRUSTER_POWER = .4f;
	const float HIGH_THRUSTER_POWER = 1.f;
}

ShipAI::ShipAI(EntityID ent, const std::string& stats) 
	: Component(ent),
	  m_shipStatsID(stats),
      m_shipStats(LoadShip(stats)),
	  m_controller(entity->GetComponent<ShipController>()),
	  m_position(entity->GetComponent<Position>())
{
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
		AttackedEvent* event{nullptr};
		if (event = entity->events.Get<EventType::Attacked>())
		{
			HandleAttackedEvent(event);
		}
	}
}

void ShipAI::ProcessAIState()
{
	switch(m_currentState)
	{
	case AIState::None:
		m_controller.SetThrusterPower(LOW_THRUSTER_POWER);
		FindStation();
		break;
	case AIState::AttackingShip:
		// target is dead or otherwise "gone"
		if (!m_targetHandle.IsValid())
		{
			m_controller.Clear();
			m_currentState = AIState::None;
		}			
		break;
	case AIState::MovingToStation:
		if (m_targetHandle.IsValid())
		{
			float dist = (m_position - *m_stationPosition).LengthSquared();
			if (dist < m_shipStats->GetApproachDistance() * m_shipStats->GetApproachDistance() * 1.15f)
			{
				m_lastStationReached = m_targetHandle.GetID();
				m_currentState = AIState::None;
			}
		}
		else m_currentState = AIState::None;
		break;
	}
}

void ShipAI::HandleAttackedEvent(AttackedEvent* event)
{
	if (!EntityManager::IsValidID(event->attackerID))
		return;

	auto ent = EntityManager::Get(event->attackerID);

	m_targetHandle = ent;

	m_controller.SetTarget(event->attackerID);
	m_controller.Set(StrafeToTargetsRearForAttack);
	m_controller.Set(FireGunsWhenFacingTarget);
	m_controller.SetThrusterPower(HIGH_THRUSTER_POWER);

	m_currentState = AIState::AttackingShip;
}

void ShipAI::FindStation()
{
	auto& stations = EntityManager::GetEntitiesByGroup(STATION_GROUP);

	int index = rand() % stations.size();
	while (stations[index]->GetID() == m_lastStationReached)
		index = rand() % stations.size();

	Entity* station = stations[index];
	/*Entity* station = EntityHelpers::GetClosestEntity(entity, stations, [this](Entity* e) { return e->GetID() != m_lastStationReached; });
	if (station == nullptr)
		return;*/

	m_targetHandle = EntityManager::Get(station->GetID());
	m_stationPosition = &m_targetHandle->GetComponent<Position>();
	m_controller.SetTarget(m_targetHandle->GetID());
	m_controller.Set(Maneuvers::Approach);
	m_controller.SetThrusterPower(LOW_THRUSTER_POWER);

	m_currentState = AIState::MovingToStation;
}
