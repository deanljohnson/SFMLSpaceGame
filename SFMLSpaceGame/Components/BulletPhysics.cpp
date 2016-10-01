#include <Components/BulletPhysics.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Entity.h>

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
	
	m_body = entity->GetWorld().CreateBody(&bodyDef);
	
	b2FixtureDef fixDef;
	fixDef.density = 1.f;
	fixDef.isSensor = true;
	b2PolygonShape shape;
	shape.SetAsBox(m_size.x / 2.f, m_size.y / 2.f);
	fixDef.shape = &shape;
	
	m_body->CreateFixture(&fixDef);
}

void BulletPhysics::Update()
{
	m_position->position = m_body->GetPosition();
	m_rotation->SetRadians(m_body->GetAngle());

	m_body->SetLinearVelocity(b2Vec2(cosf(m_speed), -sinf(m_speed)));
}