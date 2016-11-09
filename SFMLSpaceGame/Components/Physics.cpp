#include "stdafx.h"
#include <Components/Physics.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif
#include <GameState.h>
#include <CollisionGroups.h>

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

Physics::Physics(EntityID ent)
	: Physics(ent, b2_dynamicBody, 1.0f)
{}

Physics::Physics(EntityID ent, b2BodyType t, float linearDamping)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_bodyType(t),
	  m_linDamping(linearDamping)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.X(), m_position.Y());
	bodyDef.angle = m_rotation.GetRadians();
	bodyDef.type = m_bodyType;
	bodyDef.fixedRotation = false;
	bodyDef.linearDamping = m_linDamping;

	m_body = GameState::world.CreateBody(&bodyDef);

	m_body->SetUserData(entity.GetRawPointer());
}

Physics::~Physics()
{
	if (m_body != nullptr)
		GameState::world.DestroyBody(m_body);
}

void Physics::Update()
{
	m_position.position = m_body->GetPosition();
	m_rotation.SetRadians(m_body->GetAngle());
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

b2Vec2 Physics::GetHeading()
{
	return Rotate(b2Vec2(1, 0), b2Rot(GetRotationRadians()));
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

void Physics::AddShape(const sf::Shape& s, float density, int categoryBits, int collidesWithBits)
{
	// Convert SFML shape points into b2Vec2's
	b2Vec2* points = new b2Vec2[s.getPointCount()];
	for (size_t i = 0; i < s.getPointCount(); i++)
	{
		points[i] = SFMLVecToB2Vec(s.getPoint(i) - s.getOrigin());
	}

	b2PolygonShape poly;
	poly.Set(points, s.getPointCount());

	b2FixtureDef fixDef;
	fixDef.density = density;
	fixDef.shape = &poly;
	fixDef.filter.categoryBits = categoryBits;
	fixDef.filter.maskBits = collidesWithBits;
	fixDef.isSensor = (categoryBits & IS_SENSOR) > 0;

	auto f = m_body->CreateFixture(&fixDef);
}