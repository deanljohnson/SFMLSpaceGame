#include "ShipController.h"
#include <Entity.h>
#include <ShipManeuvers.h>

void ShipController::Init()
{
	m_physics = &entity->GetComponent<Physics>();
	m_thrusters = &entity->GetComponent<ShipThrusters>();
	m_dirGuns = &entity->GetComponent<DirectionalGun>();
}

void ShipController::Update()
{
	if (m_activeBehaviours[Follow]) FollowTarget();
	if (m_activeBehaviours[Intercept]) InterceptTarget();
	if (m_activeBehaviours[Approach]) ApproachTarget();
	if (m_activeBehaviours[Maneuvers::FaceTargetAndFireDirectionalGuns]) FaceTargetAndFireDirectionalGuns();
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

void ShipController::FaceTargetAndFireDirectionalGuns()
{
	assert(m_target != nullptr);
	ShipManeuvers::FaceTargetForAttack(m_physics, m_thrusters, m_target, 15.f * .90f);

	m_dirGuns->Shoot();
}

void ShipController::Set(Maneuvers maneuver, bool val)
{
	m_activeBehaviours.set(maneuver, val);
}

void ShipController::SetTarget(Entity* target)
{
	assert(target->HasComponent<Physics>());
	m_target = &target->GetComponent<Physics>();
}
