#include "Physics.h"
#include "Entity.h"
#include "Time.h"
#include "VectorMath.h"
#include <Box2D/Dynamics/b2Body.h>

void Physics::Init()
{
	m_position = &entity->GetComponent<Position>();

	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position->X(), m_position->Y());
	bodyDef.type = m_bodyType;

	m_body = entity->GetWorld().CreateBody(&bodyDef);
}

void Physics::Update()
{
	m_position->position = B2VecToSFMLVec(m_body->GetPosition());
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