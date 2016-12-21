// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/BulletPhysics.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <ProjectileStats.h>
#include <VectorMath.h>
#include <Entity.h>
#include <CollisionGroups.h>
#include <GameState.h>

BulletPhysics::BulletPhysics(EntityID ent, EntityID sourceEnt, float speed, float damage, const b2Vec2& size)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_sourceEntity(sourceEnt),
	  m_speed(speed),
	  m_damage(damage),
	  m_size(size)
{
	CreateBody();

	m_vel = b2Vec2(cos(m_rotation.GetRadians()),
		sin(m_rotation.GetRadians())) * m_speed;
}

BulletPhysics::BulletPhysics(EntityID ent, EntityID sourceEnt, std::shared_ptr<ProjectileStats> proj)
	: BulletPhysics(ent, sourceEnt, proj->speed, proj->damage, proj->size)
{
}

BulletPhysics::~BulletPhysics()
{
	if (m_body != nullptr)
		GameState::world.DestroyBody(m_body);
}

void BulletPhysics::Update()
{
	if (HandleCollisions())
		return;

	// Store pos/rot from physics body into pos/rot components
	m_position.position = m_body->GetPosition();
	m_rotation.SetRadians(m_body->GetAngle());

	// Move the bullet forward at full speed
	m_body->SetLinearVelocity(m_vel);
}

void BulletPhysics::SetSourceEntity(EntityID sourceEnt)
{
	m_sourceEntity = sourceEnt;
}

void BulletPhysics::SetStats(std::shared_ptr<ProjectileStats> proj)
{
	m_speed = proj->speed;
	m_damage = proj->damage;
	m_size = proj->size;

	m_vel = b2Vec2(cos(m_rotation.GetRadians()),
		sin(m_rotation.GetRadians())) * m_speed;

	// Remove fixtures from projectile
	b2Fixture* fix;
	while (fix = m_body->GetFixtureList())
	{
		m_body->DestroyFixture(fix);
	}

	CreateFixture();
}

void BulletPhysics::SetPosition(const b2Vec2& pos)
{
	m_position.position = pos;
	m_body->SetTransform(pos, m_body->GetAngle());
}

void BulletPhysics::SetAngle(float radians)
{
	m_rotation.SetRadians(radians);
	m_body->SetTransform(m_body->GetPosition(), radians);
}

void BulletPhysics::OnEntityEnable()
{
	CreateBody();
}

void BulletPhysics::OnEntityDisable()
{
	if (m_body != nullptr)
		GameState::world.DestroyBody(m_body);
	m_body = nullptr;
}

bool BulletPhysics::HandleCollisions()
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
					new AttackedEvent(m_sourceEntity, m_damage, m_position.X(), m_position.Y())
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

void BulletPhysics::CreateBody()
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position.X(), m_position.Y());
	bodyDef.angle = m_rotation.GetRadians();
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;

	m_body = GameState::world.CreateBody(&bodyDef);

	CreateFixture();

	m_body->SetUserData(entity.GetRawPointer());
}

void BulletPhysics::CreateFixture()
{
	b2FixtureDef fixDef;
	fixDef.density = 1.f;
	fixDef.isSensor = true;

	fixDef.filter.categoryBits = IS_BULLET;
	fixDef.filter.maskBits = COLLIDES_WITH_SHIP | COLLIDES_WITH_STATION;
	b2PolygonShape shape;
	shape.SetAsBox(m_size.x / 2.f, m_size.y / 2.f);
	fixDef.shape = &shape;

	m_body->CreateFixture(&fixDef);
}
