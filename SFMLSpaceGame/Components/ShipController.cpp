#include "ShipController.h"
#include <Entity.h>
#include <ShipManeuvers.h>

void ShipController::Init()
{
	m_physics = &entity->GetComponent<Physics>();
	m_thrusters = &entity->GetComponent<ShipThrusters>();
}

void ShipController::Update()
{
	if (activeBehaviours[Follow]) FollowTarget();
	if (activeBehaviours[Intercept]) InterceptTarget();
	if (activeBehaviours[Approach]) ApproachTarget();
}

void ShipController::FollowTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Follow(m_physics, m_thrusters, m_target, m_followDistance);
}

void ShipController::InterceptTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Intercept(m_physics, m_thrusters, m_target, m_interceptLeadMultiplier);
}

void ShipController::ApproachTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Approach(m_physics, m_thrusters, m_target, m_approachDistance);
}

void ShipController::SetTarget(Entity* target)
{
	assert(target->HasComponent<Physics>());
	m_target = &target->GetComponent<Physics>();
}
