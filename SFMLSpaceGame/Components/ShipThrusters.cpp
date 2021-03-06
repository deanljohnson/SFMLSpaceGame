// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/ShipThrusters.h>
#include <Components/Physics.h>
#include <Entity.h>
#include <VectorMath.h>
#include <ExtendedMath.h>
#include <GameTime.h>
#include <ShipStats.h>
#include <ResourceLoader.h>

b2Vec2 ShipThrust::GetMoveForce(ThrustDirection dir)
{
	switch (dir)
	{
	case Front:
		return b2Vec2(Forward, 0);
	case Left:
		return b2Vec2(0, -Side);
	case Right:
		return b2Vec2(0, Side);
	case ThrustDirection::Reverse:
		return b2Vec2(-Reverse, 0);
	case SteerLeft: break;
	case SteerRight: break;
	default: break;
	}
	return b2Vec2();
}

float ShipThrust::GetTurningForce(ThrustDirection dir)
{
	switch(dir)
	{
	case SteerLeft:
		return -Steer;
	case SteerRight:
		return Steer;
	case Front: break;
	case Left: break;
	case Right: break;
	case ThrustDirection::Reverse: break;
	default: break;
	}

	return 0.f;
}

ShipThrusters::ShipThrusters(EntityID ent, const std::string& shipID)
	: Component(ent), 
	  m_physics(entity->GetComponent<Physics>()),
	  m_shipID(shipID),
	  m_strength(LoadShip(shipID)->GetShipThrust()), 
	  m_currentTorque(0.f), 
	  m_power(1.f)
{
}

void ShipThrusters::Update()
{
	if (m_currentMoveForce.LengthSquared() != 0)
	{
		// Don't apply more thrust than the thruster strength in any direction
		m_currentMoveForce = b2Clamp(m_currentMoveForce, 
									b2Vec2(-m_strength->Reverse, -m_strength->Side) * m_power, 
									b2Vec2(m_strength->Forward, m_strength->Side) * m_power);

		b2Body* b = m_physics.GetBody();
		b->ApplyForceToCenter(Rotate(m_currentMoveForce * GameTime::deltaTime, b->GetAngle()), true);

		m_lastMoveForce = m_currentMoveForce;
		m_currentMoveForce.SetZero();
	} else m_lastMoveForce = m_currentMoveForce;
	
	if (m_currentTorque != 0.f)
	{
		// Don't steer more than the thruster strength in any direction
		m_currentTorque = std::max(-m_strength->Steer * m_power, std::min(m_currentTorque, m_strength->Steer * m_power));
		
		b2Body* b = m_physics.GetBody();
		b->ApplyTorque(m_currentTorque * GameTime::deltaTime, true);

		m_currentTorque = 0.f;
	}
}

void ShipThrusters::ApplyThrust(ThrustDirection dir, float amount)
{
	if (dir != SteerLeft && dir != SteerRight)
	{
		b2Vec2 thrust = m_strength->GetMoveForce(dir);
		m_currentMoveForce += thrust * amount;
	}
	else
	{
		float torque = m_strength->GetTurningForce(dir);
		m_currentTorque += torque * amount;
	}
}

void ShipThrusters::SetPower(float val)
{
	m_power = std::max(0.f, val);
}

float ShipThrusters::GetPower()
{
	return m_power;
}

float ShipThrusters::GetStrength(ThrustDirection dir)
{
	switch (dir)
	{
	case Front:
		return m_strength->Forward * m_power;
	case Left:
	case Right:
		return m_strength->Side * m_power;
	case ThrustDirection::Reverse:
		return m_strength->Reverse * m_power;
	case SteerLeft:
	case SteerRight:
		return m_strength->Steer * m_power;
	}

	return 0.f;
}

b2Vec2 ShipThrusters::GetCurrentForce() const
{
	return m_lastMoveForce;
}

void ShipThrusters::SteerTowardsHeading(b2Vec2 heading, float lookAheadFactor)
{
	SteerTowardsAngle(atan2f(heading.y, heading.x), lookAheadFactor);
}

void ShipThrusters::SteerTowardsAngle(float angle, float lookAheadFactor)
{
	// figure where our current angular velocity is taking us
	float nextAngle = m_physics.GetRotationRadians()
		+ (m_physics.GetAngularVelocity() / lookAheadFactor);
	float totalRotation = angle - nextAngle;

	// constrain rotation to (-M_PI, M_PI)
	totalRotation = ConstrainToPlusMinusPI(totalRotation);

	float lerpFactor = abs(totalRotation) > 3.f
		? 1.f
		: abs(totalRotation) / 3.f;

	if (totalRotation < -0.01f)
		ApplyThrust(SteerLeft, lerpFactor);
	else if (totalRotation > 0.01f)
		ApplyThrust(SteerRight, lerpFactor);
}
