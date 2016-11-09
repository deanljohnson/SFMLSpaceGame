#pragma once
#include "ShipThrusters.h"
#include "AnimatedSprite.h"

class ThrusterAnimator : public Component
{
private:
	ShipThrusters& m_thrusters;
	std::vector<AnimatedSprite*> m_thrusterSprites;

	float m_currentScale;

	void ApplyScale();
public:
	ThrusterAnimator(EntityID ent, const std::vector<AnimatedSprite*>& thrusters);

	virtual void Update() override;
};