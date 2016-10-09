#include <Components/ThrusterAnimator.h>
#include <Entity.h>
#include <ExtendedMath.h>
#include <GameTime.h>

void ThrusterAnimator::Init()
{
	m_thrusters = &entity->GetComponent<ShipThrusters>();
}

void ThrusterAnimator::Update()
{
	float force = m_thrusters->GetCurrentForce().x;
	float forwardStrength = m_thrusters->GetStrength(Front);

	force = std::max(0.f, force / forwardStrength);
	m_currentScale = Lerp(m_currentScale, force, GameTime::deltaTime * 3);

	ApplyScale();
}

void ThrusterAnimator::ApplyScale()
{
	float yScale = std::min(1.f, m_currentScale + .3f);
	m_thrusterOne->SetScale(m_currentScale, yScale);
	m_thrusterTwo->SetScale(m_currentScale, yScale);
}
