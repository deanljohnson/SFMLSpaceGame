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
	if (m_activeBehaviours[Maneuvers::FireGuns]) FireGuns();
	if (m_activeBehaviours[Maneuvers::FaceTargetAndFireDirectionalGuns]) FaceTargetAndFireDirectionalGuns();
	if (m_activeBehaviours[StrafeLeftForAttack]) StrafeForAttack(Left);
	if (m_activeBehaviours[StrafeRightForAttack]) StrafeForAttack(Right);
	if (m_activeBehaviours[StrafeToTargetsRearForAttack]) StrafeToRearForAttack();
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

void ShipController::FireGuns()
{
	m_dirGuns->Shoot();
}

void ShipController::FaceTargetAndFireDirectionalGuns()
{
	assert(m_target != nullptr);
	ShipManeuvers::FaceTargetForAttack(m_physics, m_thrusters, m_target, 15.f * .90f);
}

void ShipController::StrafeForAttack(ThrustDirection dir)
{
	assert(m_target != nullptr && (dir == Left || dir == Right));
	ShipManeuvers::StrafeAtDistanceForAttack(m_physics, m_thrusters, m_target, dir, 15.f * .90f, m_strafeDistance);
}

bool IsRight(b2Vec2 a1, b2Vec2 a2, b2Vec2 b) 
{
	return ((a2.x - a1.x)*(b.y - a1.y) - (a2.y - a1.y)*(b.x - a1.x)) > 0;
}

void ShipController::StrafeToRearForAttack() 
{
	assert(m_target != nullptr);

	b2Vec2 targetHeading = m_target->GetHeading();
	b2Vec2 targetPos = m_target->GetPosition();

	if (IsRight(m_physics->GetPosition(), targetPos, (targetPos + (-targetHeading))))
	{
		StrafeForAttack(Right);
	}
	else StrafeForAttack(Left);
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
