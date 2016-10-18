#include <Components\Shields.h>
#include <Entity.h>
#include <VectorMath.h>

void Shields::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
}

void Shields::Update() 
{

}

void Shields::SetActive(Direction dirMask)
{
	m_activationMask = dirMask;

	if (m_activationMask == Direction::Front)
	{
		m_currentFrontStrength = 
			std::min(m_data->FrontStrength, 
					m_currentFrontStrength + m_currentRearStrength + m_currentSideStrength);
		m_currentSideStrength = 0;
		m_currentRearStrength = 0;
	}
	else if (m_activationMask == Direction::Side) 
	{
		m_currentSideStrength =
			std::min(m_data->SideStrength,
				m_currentFrontStrength + m_currentRearStrength + m_currentSideStrength);
		m_currentFrontStrength = 0;
		m_currentRearStrength = 0;
	}
	else if (m_activationMask == Direction::Rear)
	{
		m_currentRearStrength =
			std::min(m_data->RearStrength,
				m_currentFrontStrength + m_currentRearStrength + m_currentSideStrength);
		m_currentFrontStrength = 0;
		m_currentSideStrength = 0;
	}
	else // all active
	{
		float totalCurrentStrength = m_currentFrontStrength + m_currentRearStrength + m_currentSideStrength;
		m_currentFrontStrength = std::min(m_data->FrontStrength, totalCurrentStrength / 3.f);
		m_currentSideStrength = std::min(m_data->SideStrength, totalCurrentStrength / 3.f);
		m_currentRearStrength = std::min(m_data->RearStrength, totalCurrentStrength / 3.f);
	}
}

void Shields::Modify(Event& event) 
{
	b2Vec2 collisionPos{event.attacked.collisionX, event.attacked.collisionY};

	b2Rot rot{ m_rotation->GetRadians() };

	auto rotatedPos = Rotate(m_position->position, rot);
	auto rotatedCollisionPoint = Rotate(collisionPos, rot);

	auto dif = rotatedCollisionPoint - rotatedPos;

	if (dif.x > 0 
		&& dif.x > abs(dif.y) 
		&& ((int)m_activationMask & (int)Direction::Front > 0))
		AbsorbFrontDamage(event);
	else if (dif.x < 0 
		&& abs(dif.x) > abs(dif.y)
		&& ((int)m_activationMask & (int)Direction::Rear > 0))
		AbsorbRearDamage(event);
	else if ((int)m_activationMask & (int)Direction::Side > 0)
		AbsorbSideDamage(event);
	else
	{
		// didn't hit an active shield, take full damage
		return;
	}
}

void Shields::AbsorbFrontDamage(Event& event) 
{
	float newShieldStrength = m_currentFrontStrength - event.attacked.damage;

	if (newShieldStrength > 0) 
	{
		// full absorption
		m_currentFrontStrength = newShieldStrength;
		event.attacked.damage = 0;
	}
	else
	{ 
		float damageTaken = abs(newShieldStrength);
		event.attacked.damage = damageTaken;
		OnShieldDeplete();
	}
}

void Shields::AbsorbSideDamage(Event& event)
{
	float newShieldStrength = m_currentSideStrength - event.attacked.damage;

	if (newShieldStrength > 0)
	{
		// full absorption
		m_currentSideStrength = newShieldStrength;
		event.attacked.damage = 0;
	}
	else
	{
		float damageTaken = abs(newShieldStrength);
		event.attacked.damage = damageTaken;
		OnShieldDeplete();
	}
}

void Shields::AbsorbRearDamage(Event& event)
{
	float newShieldStrength = m_currentRearStrength - event.attacked.damage;

	if (newShieldStrength > 0)
	{
		// full absorption
		m_currentRearStrength = newShieldStrength;
		event.attacked.damage = 0;
	}
	else
	{
		float damageTaken = abs(newShieldStrength);
		event.attacked.damage = damageTaken;
		OnShieldDeplete();
	}
}

void Shields::OnShieldDeplete() 
{

}
