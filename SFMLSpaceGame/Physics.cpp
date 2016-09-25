#include "Physics.h"
#include "Entity.h"
#include "Time.h"
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
	m_position->position += velocity * Time::deltaTime;
}