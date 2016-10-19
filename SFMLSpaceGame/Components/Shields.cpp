#include <Components\Shields.h>
#include <Entity.h>
#include <VectorMath.h>
#include <GameTime.h>

void Shields::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_currentFrontStrength = m_data->FrontStrength;
	m_currentSideStrength = m_data->SideStrength;
	m_currentRearStrength = m_data->RearStrength;
}

void Shields::Update() 
{
	float regen = GameTime::deltaTime * m_data->RegenSpeed;
	switch (m_activationMask)
	{
	case Direction::Front:
		m_currentFrontStrength = std::min(m_data->FrontStrength, m_currentFrontStrength + regen);
		break;
	case Direction::Side: 
		m_currentSideStrength = std::min(m_data->SideStrength, m_currentSideStrength + regen);
		break;
	case Direction::Rear: 
		m_currentRearStrength = std::min(m_data->RearStrength, m_currentRearStrength + regen);
		break;
	case Direction::All: 
		m_currentFrontStrength = std::min(m_data->FrontStrength / 3.f, m_currentFrontStrength + regen);
		m_currentSideStrength = std::min(m_data->SideStrength / 3.f, m_currentSideStrength + regen);
		m_currentRearStrength = std::min(m_data->RearStrength / 3.f, m_currentRearStrength + regen);
		break;
	}
}

void Shields::SetActive(Direction dirMask)
{
	m_activationMask = dirMask;

	float strengthSum = m_currentFrontStrength + m_currentRearStrength + m_currentSideStrength;

	if (m_activationMask == Direction::Front)
	{
		m_currentFrontStrength = std::min(m_data->FrontStrength, strengthSum);
		m_currentSideStrength = 0;
		m_currentRearStrength = 0;
	}
	else if (m_activationMask == Direction::Side) 
	{
		m_currentSideStrength = std::min(m_data->SideStrength, strengthSum);
		m_currentFrontStrength = 0;
		m_currentRearStrength = 0;
	}
	else if (m_activationMask == Direction::Rear)
	{
		m_currentRearStrength = std::min(m_data->RearStrength, strengthSum);
		m_currentFrontStrength = 0;
		m_currentSideStrength = 0;
	}
	else // all active
	{
		m_currentFrontStrength = std::min(m_data->FrontStrength, strengthSum / 3.f);
		m_currentSideStrength = std::min(m_data->SideStrength, strengthSum / 3.f);
		m_currentRearStrength = std::min(m_data->RearStrength, strengthSum / 3.f);
	}
}

void Shields::Modify(Event& event) 
{
	b2Vec2 collisionPos{event.attacked.collisionX, event.attacked.collisionY};

	// calculate the localized difference between the contact 
	// point and our current location
	auto dif = collisionPos - m_position->position;
	b2Rot rot{ -m_rotation->GetRadians() };
	dif = Rotate(dif, rot);
	
	if (dif.x > 0 
		&& dif.x > abs(dif.y) 
		&& ((static_cast<int>(m_activationMask) & static_cast<int>(Direction::Front)) > 0))
	{
		AbsorbFrontDamage(event);
		if (m_shieldHitCallback != nullptr)
			m_shieldHitCallback(Direction::Front, dif);
	}
	else if (dif.x < 0
		&& abs(dif.x) > abs(dif.y)
		&& ((static_cast<int>(m_activationMask) & static_cast<int>(Direction::Rear)) > 0))
	{
		AbsorbRearDamage(event);
		if (m_shieldHitCallback != nullptr)
			m_shieldHitCallback(Direction::Rear, dif);
	}
	else if ((static_cast<int>(m_activationMask) & static_cast<int>(Direction::Side)) > 0)
	{
		AbsorbSideDamage(event);
		if (m_shieldHitCallback != nullptr)
			m_shieldHitCallback(Direction::Side, dif);
	}
	else
	{
		// didn't hit an active shield, take full damage
		return;
	}
}

void Shields::SetShieldHitCallback(std::function<void(Direction, const b2Vec2&)> callback)
{
	m_shieldHitCallback = callback;
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

float Shields::GetMaxStrength(Direction dir) 
{
	switch (dir) 
	{
	case Direction::Front:
		return m_data->FrontStrength;
	case Direction::Side:
		return m_data->SideStrength;
	case Direction::Rear:
		return m_data->RearStrength;
	case Direction::All:
		return m_data->FrontStrength + m_data->SideStrength + m_data->RearStrength;
	}
}

float Shields::GetCurrentStrength(Direction dir) 
{
	switch (dir)
	{
	case Direction::Front:
		return m_currentFrontStrength;
	case Direction::Side:
		return m_currentSideStrength;
	case Direction::Rear:
		return m_currentRearStrength;
	case Direction::All:
		return m_currentFrontStrength + m_currentSideStrength + m_currentRearStrength;
	}
}
