#include "Physics.h"
#include "Entity.h"
#include "Time.h"
#include "VectorMath.h"
#include "WorldConstants.h"
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void Physics::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position->X(), m_position->Y());
	bodyDef.type = m_bodyType;
	bodyDef.fixedRotation = false;
	
	m_body = entity->GetWorld().CreateBody(&bodyDef);
}

void Physics::Update()
{
	m_position->position = B2VecToSFMLVec(m_body->GetPosition());
	m_rotation->SetRadians(m_body->GetAngle());
}

void Physics::SetPosition(const b2Vec2& v) 
{
	m_body->SetTransform(v, m_body->GetAngle());
}

const b2Vec2 Physics::GetPosition() const
{
	return m_body->GetPosition();
}

void Physics::SetVelocity(const b2Vec2& v) 
{
	m_body->SetLinearVelocity(v);
}

const b2Vec2 Physics::GetVelocity() const
{
	return m_body->GetLinearVelocity();
}

void Physics::RotateTowards(const b2Vec2& pos, float torqueScale, float smoothingScale) 
{
	b2Vec2 difVector = pos - m_body->GetPosition();
	float targetAngle = (atan2f(difVector.y, difVector.x));

	//the next angle we will be at. Look ahead 1/3 of a second to smooth the rotation well
	float nextAngle = m_body->GetAngle() + m_body->GetAngularVelocity() / (3.f * smoothingScale);
	float totalRotation = targetAngle - nextAngle;

	if (sgn(targetAngle) != sgn(nextAngle))
		totalRotation = abs(targetAngle - nextAngle) * (sgn(targetAngle));

	float lerpFactor = (totalRotation > 3.f ? totalRotation : (totalRotation * (totalRotation / 3.f)));
	m_body->ApplyTorque((totalRotation < 0 ? -torqueScale : torqueScale) * lerpFactor, true);
}

void Physics::AddShape(const sf::Shape& s, float density)
{
	b2Vec2* points = new b2Vec2[s.getPointCount()];
	for (size_t i = 0; i < s.getPointCount(); i++)
	{
		points[i] = MapSFMLToWorld(s.getPoint(i) - s.getOrigin());
	}

	b2PolygonShape poly;
	poly.Set(points, s.getPointCount());

	m_body->CreateFixture(&poly, density);
}