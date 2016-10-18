#include <Components\Shields.h>
#include <Entity.h>
#include <VectorMath.h>

void Shields::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
}

void Shields::SetActive(Direction dirMask)
{
	m_activationMask = dirMask;
}

void Shields::Modify(Event& event) 
{
	b2Vec2 collisionPos{event.attacked.collisionX, event.attacked.collisionY};

	b2Rot rot{ m_rotation->GetRadians() };

	auto rotatedPos = Rotate(m_position->position, rot);
	auto rotatedCollisionPoint = Rotate(collisionPos, rot);

	auto dif = rotatedCollisionPoint - rotatedPos;

	if (dif.x > 0 && dif.x > abs(dif.y))
		ModifyForFrontHit(event);
	else if (dif.x < 0 && abs(dif.x) > abs(dif.y))
		ModifyForRearHit(event);
	else 
		ModifyForSideHit(event);
}

void Shields::ModifyWithAmount(Event& event, float amount) const
{
	event.attacked.damage *= amount / 100.f;
}

void Shields::ModifyForFrontHit(Event& event) const
{
	if (((int)m_activationMask & (int)Direction::Front) > 0)
	{
		ModifyWithAmount(event, 100.f - 
			(((int)m_activationMask & (int)Direction::All > 0)
			? m_data->FrontStrength / 3.f
			: m_data->FrontStrength));
	}
}

void Shields::ModifyForSideHit(Event& event) const
{
	if (((int)m_activationMask & (int)Direction::Side) > 0)
	{
		ModifyWithAmount(event, 100.f -
			(((int)m_activationMask & (int)Direction::All > 0)
			? m_data->SideStrength / 3.f
			: m_data->SideStrength));
	}
}

void Shields::ModifyForRearHit(Event& event) const
{
	if (((int)m_activationMask & (int)Direction::Rear) > 0)
	{
		ModifyWithAmount(event, 100.f -
			(((int)m_activationMask & (int)Direction::All > 0)
			? m_data->RearStrength / 3.f
			: m_data->RearStrength));
	}
}

