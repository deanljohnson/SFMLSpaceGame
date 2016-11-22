#include "stdafx.h"
#include <Components\MissileController.h>
#include <EntityManager.h>
#include <VectorMath.h>
#include <ExtendedMath.h>
#include <GameTime.h>
#include <MissileStats.h>

MissileController::MissileController(EntityID ent, EntityID source, EntityID target, const std::string& projID)
	: Component(ent),
	  STEER_FORCE(4.f),
	  m_physics(entity->GetComponent<MissilePhysics>()),
	  m_sourceEntity(source),
	  m_targetEntity(target),
	  m_missStats(LoadMissile(projID)),
	  m_projID(projID)
{
	if (EntityManager::IsValidID(m_targetEntity))
	{
		m_targetHandle = EntityManager::Get(m_targetEntity);
	}
}

void MissileController::SteerLeft(float amount) 
{
	float torque = -STEER_FORCE;
	m_currentTorque += torque * amount;
}

void MissileController::SteerRight(float amount) 
{
	float torque = STEER_FORCE;
	m_currentTorque += torque * amount;
}

void MissileController::Update()
{
	const float lookAheadFactor = 6.f;
	bool facingTarget = true;

	if (EntityManager::IsValidID(m_targetEntity)) 
	{
		auto targetPos = m_targetHandle->GetComponent<Position>().position;

		auto seekVec = (targetPos - m_physics.GetPosition());
		seekVec.Normalize();
		auto seekAngle = atan2f(seekVec.y, seekVec.x);

		facingTarget = WithinAngle(m_physics.GetHeading(), seekVec, COS_15);

		// figure where our current angular velocity is taking us
		float nextAngle = m_physics.GetRotationRadians()
			+ (m_physics.GetAngularVelocity() / lookAheadFactor);
		float totalRotation = seekAngle - nextAngle;

		totalRotation = ConstrainToPlusMinusPI(totalRotation);

		float lerpFactor = abs(totalRotation) > 3.f
			? 1.f
			: abs(totalRotation) / 3.f;

		if (totalRotation < -0.01f)
			SteerLeft(lerpFactor);
		else if (totalRotation > 0.01f)
			SteerRight(lerpFactor);
	}

	b2Body* body = m_physics.GetBody();

	// apply forward thrust, if roughly facing target
	if (facingTarget)
		body->ApplyForceToCenter(m_physics.GetHeading() * m_missStats->GetThrust(), true);

	// turn as needed
	if (m_currentTorque != 0.f) 
	{
		// Don't steer more than the thruster strength in any direction
		m_currentTorque = std::max(-STEER_FORCE, std::min(m_currentTorque, STEER_FORCE));

		b2Body* b = m_physics.GetBody();
		b->ApplyTorque(m_currentTorque * GameTime::deltaTime, true);

		m_currentTorque = 0.f;
	}
}