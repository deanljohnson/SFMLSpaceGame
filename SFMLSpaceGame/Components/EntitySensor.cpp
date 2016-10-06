#include <Components/EntitySensor.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <Box2D/Collision/Shapes/b2CircleShape.h>

void EntitySensor::Init() 
{
	m_physics = &entity->GetComponent<Physics>();

	b2FixtureDef fixDef;
	fixDef.isSensor = true;
	b2CircleShape shape;
	shape.m_radius = m_radius;
	fixDef.shape = &shape;

	m_physics->GetBody()->CreateFixture(&fixDef);
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
		auto otherEnt = contactList->other->GetUserData();
		if (otherEnt != nullptr)
			HandleCollisionWithEntity(static_cast<Entity*>(otherEnt));

		contactList = contactList->next;
	}
}

void EntitySensor::HandleCollisionWithEntity(Entity* ent) 
{
	// If the entity is of the right group, trigger the sensor
	if (!ent->HasGroup(m_group))
		return;

	sensedEntities.push_back(ent);
}