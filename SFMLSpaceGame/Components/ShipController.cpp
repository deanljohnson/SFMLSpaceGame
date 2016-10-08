#include "ShipController.h"
#include <Entity.h>
#include <ShipManeuvers.h>
#include <VectorMath.h>
#include <ExtendedMath.h>
#include <assert.h>

void ShipController::Init()
{
	m_physics = &entity->GetComponent<Physics>();
	m_thrusters = &entity->GetComponent<ShipThrusters>();
	m_dirGuns = &entity->GetComponent<DirectionalGun>();
}

void ShipController::Update()
{
	// Run any active behavior
	if (m_activeBehaviours[Follow]) FollowTarget();
	if (m_activeBehaviours[Intercept]) InterceptTarget();
	if (m_activeBehaviours[Approach]) ApproachTarget();
	if (m_activeBehaviours[Maneuvers::FireGuns]) FireGuns();
	if (m_activeBehaviours[Maneuvers::FireGunsWhenFacingTarget]) FireGunsWhenFacingTarget();
	if (m_activeBehaviours[Maneuvers::FaceTargetForAttack]) FaceTargetForAttack();
	if (m_activeBehaviours[StrafeLeftForAttack]) StrafeForAttack(Left);
	if (m_activeBehaviours[StrafeRightForAttack]) StrafeForAttack(Right);
	if (m_activeBehaviours[StrafeToTargetsRearForAttack]) StrafeToRearForAttack();
}

void ShipController::FollowTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Follow(m_physics, m_thrusters, m_target, m_stats->GetFollowDistance());
}

void ShipController::InterceptTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Intercept(m_physics, m_thrusters, m_target, m_stats->GetInterceptLeadMultiplier());
}

void ShipController::ApproachTarget()
{
	assert(m_target != nullptr);
	ShipManeuvers::Approach(m_physics, m_thrusters, m_target, m_stats->GetApproachDistance());
}

void ShipController::FireGuns()
{
	m_dirGuns->Shoot();
}

void ShipController::FireGunsWhenFacingTarget() 
{
	assert(m_target != nullptr);
	b2Vec2 heading = m_physics->GetHeading();
	b2Vec2 toTarget = m_target->GetPosition() - m_physics->GetPosition();
	toTarget.Normalize();

	// If heading is within 15 degrees of the toTarget vector
	if (b2Dot(heading, toTarget) > COS_15)
		FireGuns();
}

void ShipController::FaceTargetForAttack()
{
	assert(m_target != nullptr);
	ShipManeuvers::FaceTargetForAttack(m_physics, m_thrusters, m_target, 15.f * .90f);

	FireGuns();
}

void ShipController::StrafeForAttack(ThrustDirection dir)
{
	assert(m_target != nullptr && (dir == Left || dir == Right));
	ShipManeuvers::StrafeAtDistanceForAttack(m_physics, m_thrusters, m_target, dir, 15.f * .90f, m_stats->GetStrafeDistance());
}

void ShipController::StrafeToRearForAttack() 
{
	assert(m_target != nullptr);

	b2Vec2 targetPos = m_target->GetPosition();

	if (IsRight(m_physics->GetPosition(), targetPos, (targetPos -m_target->GetHeading())))
	{
		StrafeForAttack(Right);
	}
	else StrafeForAttack(Left);
}

void ShipController::Set(Maneuvers maneuver, bool val)
{
	m_activeBehaviours.set(maneuver, val);
}

void ShipController::SetTarget(EntityHandle& target)
{
	assert(target->HasComponent<Physics>());
	m_target = &target->GetComponent<Physics>();
}
