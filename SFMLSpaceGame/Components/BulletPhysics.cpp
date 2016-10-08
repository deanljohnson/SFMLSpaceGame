#include <Components/BulletPhysics.h>
#include <VectorMath.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Entity.h>
#include <Box2D/Dynamics/Contacts/b2Contact.h>
#include <CollisionGroups.h>
#include <GameState.h>

BulletPhysics::~BulletPhysics()
{
	if (m_body != nullptr)
		GameState::world.DestroyBody(m_body);
}

void BulletPhysics::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
	
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_position->X(), m_position->Y());
	bodyDef.angle = m_rotation->GetRadians();
	bodyDef.fixedRotation = true;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	
	m_body = GameState::world.CreateBody(&bodyDef);
	
	b2FixtureDef fixDef;
	fixDef.density = 1.f;
	fixDef.isSensor = true;
	//fixDef.filter.groupIndex = BULLET_COLLISION_GROUP;
	fixDef.filter.categoryBits = IS_BULLET;
	fixDef.filter.maskBits = COLLIDES_WITH_SHIP | COLLIDES_WITH_STATION;
	b2PolygonShape shape;
	shape.SetAsBox(m_projStats->GetSize().x / 2.f, m_projStats->GetSize().y / 2.f);
	fixDef.shape = &shape;
	
	m_body->CreateFixture(&fixDef);

	m_body->SetUserData(entity);
}

void BulletPhysics::Update()
{
	if (HandleCollisions())
		return;

	// Store pos/rot from physics body into pos/rot components
	m_position->position = m_body->GetPosition();
	m_rotation->SetRadians(m_body->GetAngle());

	// TODO: Cache the velocity vector to avoid cos/sin being repeatedly called
	// Move the bullet forward at full speed
	m_body->SetLinearVelocity(b2Vec2(cos(m_rotation->GetRadians()), sin(m_rotation->GetRadians())) * m_projStats->GetSpeed());
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
		if (!contact->contact->GetFixtureA()->IsSensor()
			|| !contact->contact->GetFixtureB()->IsSensor())
		{
			auto userData = contact->other->GetUserData();
			if (userData != nullptr) // If contact is not with a body with defined entity data
			{
				auto otherEnt = static_cast<Entity*>(userData);
				Event attackedEvent;
				attackedEvent.attacked = Event::AttackedEvent();
				attackedEvent.attacked.attackerID = m_sourceEntity;
				attackedEvent.attacked.damage = m_projStats->GetDamage();
				attackedEvent.type = Attacked;
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

