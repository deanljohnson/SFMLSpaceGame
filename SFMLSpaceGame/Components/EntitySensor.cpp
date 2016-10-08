#include <Components/EntitySensor.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>
#include <CollisionGroups.h>

void EntitySensor::Init() 
{
	m_physics = &entity->GetComponent<Physics>();

	b2FixtureDef fixDef;
	fixDef.isSensor = true;
	fixDef.filter.categoryBits = IS_SENSOR;
	fixDef.filter.maskBits = COLLIDES_WITH_SHIP | COLLIDES_WITH_STATION;
	b2CircleShape shape;
	shape.m_radius = m_radius;
	fixDef.shape = &shape;

	m_sensingFixture = m_physics->GetBody()->CreateFixture(&fixDef);
}

void EntitySensor::Update() 
{
	sensedEntities.clear();

	auto contactList = m_physics->GetBody()->GetContactList();

	if (contactList == nullptr
		|| contactList->other == nullptr) {
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