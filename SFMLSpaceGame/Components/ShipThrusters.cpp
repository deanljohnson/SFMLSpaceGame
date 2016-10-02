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

void ShipThrusters::ApplyThrust(ThrustDirection dir, float amount)
{
	b2Body* b = m_physics->GetBody();
	
	if (dir != SteerLeft && dir != SteerRight)
	{
		b2Vec2 thrust = m_strength.GetMoveForce(dir);
		b->ApplyForceToCenter(Rotate(thrust * amount, b->GetAngle()), true);
	}
	else
	{
		float torque = m_strength.GetTurningForce(dir);
		b->ApplyTorque(torque * amount, true);
	}
}
