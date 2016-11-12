#pragma once
#include "ShipThrusters.h"
#include "AnimatedSprite.h"
#include <DefaultSerializeable.h>

class ThrusterAnimator : public Component, public DefaultSerializeable<ThrusterAnimator>
{
private:
	ShipThrusters& m_thrusters;
	std::vector<AnimatedSprite*> m_thrusterSprites;

	float m_currentScale;

	void ApplyScale();
public:
	explicit ThrusterAnimator(EntityID ent);

	virtual void Update() override;

	void AddSprite(AnimatedSprite* sprite);
};