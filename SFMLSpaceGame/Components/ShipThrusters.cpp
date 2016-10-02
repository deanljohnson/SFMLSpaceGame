#include <Components/ShipThrusters.h>
#include <Entity.h>
#include <VectorMath.h>

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
	}

	return 0.f;
}

void ShipThrusters::Init()
{
	m_physics = &entity->GetComponent<Physics>();
}

void ShipThrusters::Update()
{
	if (m_currentMoveForce.LengthSquared() != 0)
	{
		m_currentMoveForce = b2Clamp(m_currentMoveForce, 
									b2Vec2(-m_strength.Reverse, -m_strength.Side), 
									b2Vec2(m_strength.Forward, m_strength.Side));

		b2Body* b = m_physics->GetBody();
		b->ApplyForceToCenter(Rotate(m_currentMoveForce, b->GetAngle()), true);

		m_currentMoveForce.SetZero();
	}
	
	if (m_currentTorque != 0.f)
	{
		m_currentTorque = std::max(-m_strength.Steer, std::min(m_currentTorque, m_strength.Steer));
		
		b2Body* b = m_physics->GetBody();
		b->ApplyTorque(m_currentTorque, true);
		m_currentTorque = 0.f;
	}
}

void ShipThrusters::ApplyThrust(ThrustDirection dir, float amount)
{
	if (dir != SteerLeft && dir != SteerRight)
	{
		b2Vec2 thrust = m_strength.GetMoveForce(dir);
		m_currentMoveForce += thrust * amount;
	}
	else
	{
		float torque = m_strength.GetTurningForce(dir);
		m_currentTorque += torque * amount;
	}
}
