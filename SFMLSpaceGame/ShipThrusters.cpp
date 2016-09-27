#include "ShipThrusters.h"
#include "Entity.h"
#include "VectorMath.h"

b2Vec2 ShipThrust::Get(ThrustDirection dir)
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
		return b2Vec2(-Forward, 0);;
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

	b2Vec2 thrust = Rotate(m_strength.Get(dir), b->GetAngle());
	b2Vec2 curVel = Rotate(b->GetLinearVelocity(), b->GetAngle());
	
	curVel += thrust;
	curVel = b2Clamp(curVel, b2Vec2(m_maxStrength.Reverse, -m_maxStrength.Side), b2Vec2(m_maxStrength.Forward, m_maxStrength.Side));

	b->SetLinearVelocity(curVel);
}
