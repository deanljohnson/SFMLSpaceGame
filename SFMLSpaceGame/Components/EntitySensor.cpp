// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/EntitySensor.h>
#include <Components/Physics.h>
#include <CollisionGroups.h>
#include <Entity.h>

EntitySensor::EntitySensor(EntityID ent, float radius, const std::vector<Group>& groups)
	: Component(ent), 
	  m_physics(entity->GetComponent<Physics>()), 
	  m_groups(groups),
	  m_radius(radius)
{
	b2FixtureDef fixDef;
	fixDef.isSensor = true;
	fixDef.filter.categoryBits = IS_SENSOR;
	fixDef.filter.maskBits = COLLIDES_WITH_SHIP | COLLIDES_WITH_STATION;
	b2CircleShape shape;
	shape.m_radius = m_radius;
	fixDef.shape = &shape;

	m_sensingFixture = m_physics.GetBody()->CreateFixture(&fixDef);
}

void EntitySensor::Update() 
{
	sensedEntities.clear();

	auto contactList = m_physics.GetBody()->GetContactList();

	if (contactList == nullptr
		|| contactList->other == nullptr) {
		SetTriggered(false);
		return;
	}

	// Walk through all current contacts
	while (contactList != nullptr) 
	{
		if (contactList->contact->GetFixtureA() == m_sensingFixture
			|| contactList->contact->GetFixtureB() == m_sensingFixture)
		{
			auto otherEnt = contactList->other->GetUserData();
			if (otherEnt != nullptr)
				HandleCollisionWithEntity(static_cast<Entity*>(otherEnt));
		}

		contactList = contactList->next;
	}

	SetTriggered(sensedEntities.size() > 0);
}

void EntitySensor::SetRange(float range)
{
	m_radius = range;

	b2FixtureDef fixDef;
	fixDef.isSensor = true;
	fixDef.filter.categoryBits = IS_SENSOR;
	fixDef.filter.maskBits = COLLIDES_WITH_SHIP | COLLIDES_WITH_STATION;
	b2CircleShape shape;
	shape.m_radius = m_radius;
	fixDef.shape = &shape;

	m_physics.GetBody()->DestroyFixture(m_sensingFixture);
	m_sensingFixture = m_physics.GetBody()->CreateFixture(&fixDef);
}

void EntitySensor::AddCallback(std::function<void(bool, Sensor*)> callback)
{
	m_callbacks.push_back(callback);
}

void EntitySensor::AttachComponent(Component* c)
{
	c->SetActive(m_triggered);

	AddCallback(
		[c](bool state, Sensor* sensor)
	{
		c->SetActive(state);
	});
}

void EntitySensor::HandleCollisionWithEntity(Entity* ent) 
{
	// If the entity is of the right group, trigger the sensor
	for (auto g : m_groups)
	{
		if (ent->HasGroup(g))
		{
			sensedEntities.push_back(ent);
			break;
		}
	}
}

void EntitySensor::SetTriggered(bool val) 
{
	// If the sensor just switched states
	// trigger the callbacks and pass in
	// our current state

	// no change of state
	if (m_triggered == val) return;

	m_triggered = val;

	for (auto callback : m_callbacks)
		callback(m_triggered, this);
}