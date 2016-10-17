#pragma once
#include "ShipThrusters.h"
#include "AnimatedSprite.h"

class ThrusterAnimator : public Component
{
private:
	ShipThrusters* m_thrusters{ nullptr };
	std::vector<AnimatedSprite*> m_thrusterSprites;

	float m_currentScale;

	void ApplyScale();
public:
	explicit ThrusterAnimator(const std::vector<AnimatedSprite*>& thrusters)
		: m_thrusterSprites(thrusters), m_currentScale(0)
	{}

	virtual void Init() override;
	virtual void Update() override;
};