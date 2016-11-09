#include "stdafx.h"
#include <Components/ThrusterAnimator.h>
#include <Entity.h>
#include <ExtendedMath.h>
#include <GameTime.h>

ThrusterAnimator::ThrusterAnimator(EntityID ent, const std::vector<AnimatedSprite*>& thrusters) 
	: Component(ent), 
	  m_thrusters(entity->GetComponent<ShipThrusters>()),
	  m_thrusterSprites(thrusters),
	  m_currentScale(0)
{
}

void ThrusterAnimator::Update()
{
	float force = m_thrusters.GetCurrentForce().x;
	float forwardStrength = m_thrusters.GetStrength(Front) / m_thrusters.GetPower();

	force = std::max(0.f, force / forwardStrength);
	m_currentScale = Lerp(m_currentScale, force, GameTime::deltaTime * 3);

	ApplyScale();
}

void ThrusterAnimator::ApplyScale()
{
	float yScale = std::min(1.f, m_currentScale + .3f);

	for (auto t : m_thrusterSprites)
	{
		t->SetScale(m_currentScale, yScale);
	}
}