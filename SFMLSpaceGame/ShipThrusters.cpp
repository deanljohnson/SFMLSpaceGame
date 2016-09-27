#include "ShipThrusters.h"
#include "Entity.h"
#include "VectorMath.h"

#ifndef M_PI_2f
#define M_PI_2f (float)M_PI_2
#endif

b2Vec2 ShipThrust::Get(ThrustDirection dir)
{
	switch (dir)
	{
	case Front:
		return b2Vec2(0, -Forward);
	case Left:
		return b2Vec2(-Side, 0);
	case Right:
		return b2Vec2(Side, 0);
	case ThrustDirection::Reverse:
		return b2Vec2(0, Reverse);
	}
	return b2Vec2();
}


void ShipThrusters::Init()
{
	m_physics = &entity->GetComponent<Physics>();
}

void ShipThrusters::ApplyThrust(ThrustDirection dir)
{
	b2Body* b = m_physics->GetBody();
	
	b2Vec2 thrust = m_strength.Get(dir);
	//the current velocity, irrespective of the body's angle
	b2Vec2 curVel = Rotate(b->GetLinearVelocity(), -(b->GetAngle() + M_PI_2f));

	curVel += thrust;
	curVel = b2Clamp(curVel, b2Vec2(-m_maxStrength.Side, -m_maxStrength.Forward), b2Vec2(m_maxStrength.Side, m_maxStrength.Reverse));

	b->SetLinearVelocity(Rotate(curVel, b->GetAngle() + M_PI_2f));
}
