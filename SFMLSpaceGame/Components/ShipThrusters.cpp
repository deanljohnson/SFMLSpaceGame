#include <Components/ShipThrusters.h>
#include <Entity.h>
#include <VectorMath.h>

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
		return b2Vec2(-Reverse, 0);
	}
	return b2Vec2();
}


void ShipThrusters::Init()
{
	m_physics = &entity->GetComponent<Physics>();
}

void ShipThrusters::ApplyThrust(ThrustDirection dir, float amount)
{
	b2Body* b = m_physics->GetBody();
	
	b2Vec2 thrust = m_strength.Get(dir);
	b->ApplyForceToCenter(Rotate(thrust * amount, b->GetAngle()), true);
}
