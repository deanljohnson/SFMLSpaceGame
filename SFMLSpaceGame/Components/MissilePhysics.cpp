// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/MissilePhysics.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <VectorMath.h>
#include <Entity.h>
#include <CollisionGroups.h>
#include <GameState.h>
#include <MissileStats.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif
#include <GameTime.h>

//constrains a body's angle to be in the range [0, 2PI)
inline void WrapBodyAngle(b2Body& body)
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

MissilePhysics::MissilePhysics(EntityID ent, EntityID sourceEnt, float thrust, float damage)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sourceEntity(sourceEnt),
	  m_thrust(thrust),
	  m_damage(damage)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.X(), m_position.Y());
	bodyDef.angle = m_rotation.GetRadians();
	bodyDef.fixedRotation = false;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	bodyDef.linearDamping = 1.f;

	m_body = GameState::world.CreateBody(&bodyDef);

	m_body->SetUserData(entity.GetRawPointer());
}

MissilePhysics::MissilePhysics(EntityID ent, EntityID sourceEnt, std::shared_ptr<MissileStats> missile)
	: MissilePhysics(ent, sourceEnt, missile->thrust, missile->damage)
{
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
	
	m_body->ApplyForceToCenter(m_rotation.GetHeading() * m_thrust * GameTime::deltaTime, true);
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
				std::unique_ptr<AttackedEvent> attackedEvent
				{
					new AttackedEvent(m_sourceEntity, 
									m_damage, 
									m_position.X(), 
									m_position.Y())
				};
				otherEnt->events.Push(move(attackedEvent));
			}

			break;
		}

		contact = contact->next;
	}

	// We collided with something, doesn't matter what
	entity->Destroy();
	return true;
}

void MissilePhysics::SetPosition(const b2Vec2& v)
{
	m_position.position = v;
	m_body->SetTransform(v, m_body->GetAngle());
}

b2Vec2 MissilePhysics::GetPosition() const
{
	return m_body->GetPosition();
}

b2Vec2 MissilePhysics::GetHeading()
{
	return Rotate(b2Vec2(1, 0), b2Rot(GetRotationRadians()));
}

float MissilePhysics::GetRotationRadians()
{
	WrapBodyAngle(*m_body);
	return m_body->GetAngle();
}

float MissilePhysics::GetAngularVelocity() const
{
	return m_body->GetAngularVelocity();
}

b2Body* MissilePhysics::GetBody() 
{
	return m_body;
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