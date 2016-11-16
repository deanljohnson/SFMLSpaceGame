#include "stdafx.h"
#include <Components/MissilePhysics.h>
#include <VectorMath.h>
#include <Entity.h>
#include <CollisionGroups.h>
#include <GameState.h>
#include <ResourceLoader.h>

MissilePhysics::MissilePhysics(EntityID ent, EntityID sourceEnt, const std::string& projID)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sourceEntity(sourceEnt),
	  m_missStats(LoadMissile(projID))
{
	m_projID = projID;

	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.X(), m_position.Y());
	bodyDef.angle = m_rotation.GetRadians();
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;

	m_body = GameState::world.CreateBody(&bodyDef);

	m_body->SetUserData(entity.GetRawPointer());
}

MissilePhysics::~MissilePhysics()
{
	if (m_body != nullptr)
		GameState::world.DestroyBody(m_body);
}

void MissilePhysics::Update()
{
	if (HandleCollisions())
		return;

	// Store pos/rot from physics body into pos/rot components
	m_position.position = m_body->GetPosition();
	m_rotation.SetRadians(m_body->GetAngle());

	m_body->ApplyForceToCenter(m_rotation.GetHeading() * m_missStats->GetThrust(), true);
}

bool MissilePhysics::HandleCollisions()
{
	if (m_body->GetContactList() == nullptr
		|| m_body->GetContactList()->other == nullptr)
		return false;

	auto contact = m_body->GetContactList();
	while (true)
	{
		if (contact == nullptr)
			return false;

		// If one of the fixtures is a nonsensor
		if (contact->contact->IsTouching()
			&& (!contact->contact->GetFixtureA()->IsSensor()
				|| !contact->contact->GetFixtureB()->IsSensor()))
		{
			auto userData = contact->other->GetUserData();
			// if the contact is with an entity, give it an attacked event
			if (userData != nullptr)
			{
				b2WorldManifold manifold;
				contact->contact->GetWorldManifold(&manifold);

				auto otherEnt = static_cast<Entity*>(userData);
				Event attackedEvent;
				attackedEvent.attacked = Event::AttackedEvent();
				attackedEvent.attacked.attackerID = m_sourceEntity;
				attackedEvent.attacked.damage = m_missStats->GetDamage();
				attackedEvent.attacked.collisionX = m_position.X();
				attackedEvent.attacked.collisionY = m_position.Y();
				attackedEvent.type = EventType::Attacked;
				otherEnt->events.Push(attackedEvent);
			}

			break;
		}

		contact = contact->next;
	}

	// We collided with something, doesn't matter what
	entity->Destroy();
	return true;
}

void MissilePhysics::AddShape(const sf::Shape& s, float density, int categoryBits, int collidesWithBits)
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