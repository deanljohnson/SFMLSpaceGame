#include <Components/Physics.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <VectorMath.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif

//constrains a body's angle to be in the range [0, 2PI)
void WrapBodyAngle(b2Body& body)
{
	if (body.GetAngle() >= M_TAU || body.GetAngle() < 0)
	{
		auto a = body.GetAngle();

		while (a < 0)
			a += M_TAU;
		while (a > M_TAU)
			a -= M_TAU;
		body.SetTransform(body.GetPosition(), a);
	}
}

void Physics::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position->X(), m_position->Y());
	bodyDef.angle = m_rotation->GetRadians();
	bodyDef.type = m_bodyType;
	bodyDef.fixedRotation = false;
	bodyDef.linearDamping = m_linDamping;
	
	m_body = entity->GetWorld()->CreateBody(&bodyDef);

	m_body->SetUserData(entity);
}

void Physics::Update()
{
	m_position->position = m_body->GetPosition();
	m_rotation->SetRadians(m_body->GetAngle());
}

void Physics::SetPosition(const b2Vec2& v) 
{
	m_body->SetTransform(v, m_body->GetAngle());
}

b2Vec2 Physics::GetPosition() const
{
	return m_body->GetPosition();
}

void Physics::SetVelocity(const b2Vec2& v) 
{
	m_body->SetLinearVelocity(v);
}

b2Vec2 Physics::GetVelocity() const
{
	return m_body->GetLinearVelocity();
}

float Physics::GetAngularVelocity() const
{
	return m_body->GetAngularVelocity();
}


float Physics::GetRotationRadians()
{
	WrapBodyAngle(*m_body);
	return m_body->GetAngle();
}

void Physics::AddShape(const sf::Shape& s, float density)
{
	b2Vec2* points = new b2Vec2[s.getPointCount()];
	for (size_t i = 0; i < s.getPointCount(); i++)
	{
		points[i] = SFMLVecToB2Vec(s.getPoint(i) - s.getOrigin());
	}

	b2PolygonShape poly;
	poly.Set(points, s.getPointCount());

	m_body->CreateFixture(&poly, density);
}