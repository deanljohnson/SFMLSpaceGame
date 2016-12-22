// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/ShipAI.h>
#include <Components/Position.h>
#include <Components/ShipController.h>
#include <Components/EconomyAgent.h>
#include <EntityManager.h>
#include <Event.h>
#include <Entity.h>
#include <Economy.h>
#include <FindBestPurchaseJob.h>

namespace
{
	const float LOW_THRUSTER_POWER = .4f;
	const float HIGH_THRUSTER_POWER = 1.f;
}

ShipAI::ShipAI(EntityID ent, const std::string& stats) 
	: Component(ent),
	  m_stationPosition(nullptr),
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
		FindTrade();
		break;
	case AIState::AttackingShip:
		// target is dead or otherwise "gone"
		if (!EntityManager::IsValidID(m_targetID))
		{
			m_controller.Clear();
			m_currentState = AIState::None;
		}			
		break;
	case AIState::MovingToStation:
		if (EntityManager::IsValidID(m_targetID))
		{
			float dist = (m_position - *m_stationPosition).LengthSquared();
			if (dist < m_shipStats->approachDistance * m_shipStats->approachDistance * 1.15f)
			{
				m_lastStationReached = m_targetID;
				m_controller.Clear();
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

	m_targetID = event->attackerID;

	m_controller.SetTarget(event->attackerID);
	m_controller.Clear();
	m_controller.Set(StrafeToTargetsRearForAttack);
	m_controller.Set(FireGunsWhenFacingTarget);
	m_controller.SetThrusterPower(HIGH_THRUSTER_POWER);

	m_currentState = AIState::AttackingShip;

	// Forget about any running purchase jobs
	m_findPurchaseJob = nullptr;
}

void ShipAI::FindTrade()
{
	// If we are waiting on a find purchase job
	if (m_findPurchaseJob != nullptr)
	{
		if (m_findPurchaseJob->IsFinished())
		{
			const FindPurchaseJobResult& trade = m_findPurchaseJob->GetResult();
			
			EntityHandle handle = EntityManager::Get(trade.agent->GetEntityID());
			m_stationPosition = &handle->GetComponent<Position>();
			m_controller.Clear();
			m_controller.SetTarget(handle->GetID());
			m_controller.Set(Maneuvers::Approach);
			m_controller.SetThrusterPower(LOW_THRUSTER_POWER);

			m_currentState = AIState::MovingToStation;
		}
	}
	else
	{
		// If the last station we reached is not a valid ID, we don't have a start station (empty name)
		std::string startStationName = m_lastStationReached == ENTITY_ID_NULL || !EntityManager::IsValidID(m_lastStationReached)
			? ""
			: EntityManager::Get(m_lastStationReached)->GetName();

		m_findPurchaseJob = std::make_shared<FindBestPurchaseJob>(startStationName, 2, nullptr);
		Economy::EnqueueJob(m_findPurchaseJob);
	}
}
