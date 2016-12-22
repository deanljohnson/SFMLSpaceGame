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
#include <FindBestSaleJob.h>

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

	// Forget about any running trade jobs
	m_findPurchaseJob = nullptr;
	m_findSaleJob = nullptr;
	m_purchaseResult = nullptr;
}

void ShipAI::FindTrade()
{
	// If we are waiting on a find purchase job
	if (m_findPurchaseJob != nullptr)
	{
		if (m_findPurchaseJob->IsFinished())
		{
			ProcessFinishedPurchaseJob();
		}
	}
	// If we are waiting on a find sale job
	else if (m_findSaleJob != nullptr)
	{
		if (m_findSaleJob->IsFinished())
		{
			ProcessFinishedSaleJob();
		}
	}
	else
	{
		if (m_purchaseResult == nullptr)
			CreatePurchaseJob();
		else CreateSaleJob();
	}
}

void ShipAI::CreatePurchaseJob()
{
	// If the last station we reached is not a valid ID, we don't have a start station (empty name)
	std::string startStationName = m_lastStationReached == ENTITY_ID_NULL || !EntityManager::IsValidID(m_lastStationReached)
		? ""
		: EntityManager::Get(m_lastStationReached)->GetName();

	m_findPurchaseJob = std::make_shared<FindBestPurchaseJob>(startStationName, 2, nullptr);
	Economy::EnqueueJob(m_findPurchaseJob);
}

void ShipAI::CreateSaleJob()
{
	// If the last station we reached is not a valid ID, we don't have a start station (empty name)
	std::string startStationName = m_lastStationReached == ENTITY_ID_NULL || !EntityManager::IsValidID(m_lastStationReached)
		? ""
		: EntityManager::Get(m_lastStationReached)->GetName();

	// The amount of the item we are selling that this agent has
	unsigned int amt = entity->GetComponent<EconomyAgent>().GetAmountOfItem(m_purchaseResult->type, m_purchaseResult->detail);

	auto filter = [this](const EconomyAgent& agent, Price&) -> bool
	{
		return m_targetID != agent.GetEntityID();
	};

	m_findSaleJob = std::make_shared<FindBestSaleJob>(m_purchaseResult->type, m_purchaseResult->detail, amt, 
														startStationName, 2, filter);
	Economy::EnqueueJob(m_findSaleJob);
}

void ShipAI::ProcessFinishedPurchaseJob()
{
	assert(m_findPurchaseJob->IsFinished());

	m_purchaseResult = std::make_unique<FindPurchaseJobResult>(m_findPurchaseJob->GetResult());
	m_targetID = m_purchaseResult->agent->GetEntityID();

	EntityHandle handle = EntityManager::Get(m_targetID);
	m_stationPosition = &handle->GetComponent<Position>();
	m_controller.Clear();
	m_controller.SetTarget(m_targetID);
	m_controller.Set(Maneuvers::Approach);
	m_controller.SetThrusterPower(LOW_THRUSTER_POWER);

	m_currentState = AIState::MovingToStation;

	m_findPurchaseJob = nullptr;
}

void ShipAI::ProcessFinishedSaleJob()
{
	assert(m_findSaleJob->IsFinished());

	const FindSaleJobResult& trade = m_findSaleJob->GetResult();
	m_targetID = trade.agent->GetEntityID();

	EntityHandle handle = EntityManager::Get(m_targetID);
	m_stationPosition = &handle->GetComponent<Position>();
	m_controller.Clear();
	m_controller.SetTarget(m_targetID);
	m_controller.Set(Maneuvers::Approach);
	m_controller.SetThrusterPower(LOW_THRUSTER_POWER);

	m_currentState = AIState::MovingToStation;

	m_findSaleJob = nullptr;
}