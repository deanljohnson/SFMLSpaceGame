#pragma once
#include "ShipThrusters.h"
#include "AnimatedSprite.h"

class ThrusterAnimator : public Component
{
private:
	ShipThrusters* m_thrusters{ nullptr };
	AnimatedSprite* m_thrusterOne{ nullptr };
	AnimatedSprite* m_thrusterTwo{ nullptr };

	float m_currentScale;

	void ApplyScale();
public:
	explicit ThrusterAnimator(AnimatedSprite* thruster)
		: m_thrusterOne(thruster), m_currentScale(0.f)
	{}
	ThrusterAnimator(AnimatedSprite* thrusterOne, AnimatedSprite* thrusterTwo)
		: m_thrusterOne(thrusterOne), m_thrusterTwo(thrusterTwo), m_currentScale(0.f)
	{}

	virtual void Init() override;
	virtual void Update() override;
};