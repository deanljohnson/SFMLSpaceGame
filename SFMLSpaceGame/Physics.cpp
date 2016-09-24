#include "Physics.h"
#include "Entity.h"
#include "Time.h"

void Physics::Init()
{
	m_position = &entity->GetComponent<Position>();
}

void Physics::Update()
{
	m_position->position += velocity * Time::deltaTime;
}