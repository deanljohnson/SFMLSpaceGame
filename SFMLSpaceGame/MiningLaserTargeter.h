#pragma once
#include <Targeter.h>

struct b2Vec2;

class MiningLaserTargeter : public Targeter 
{
private:
	float m_detectionRange;
public:
	explicit MiningLaserTargeter(float detectionRange);
	virtual b2Vec2 FindTarget(const b2Vec2& pos, EntityID source) const override;

	// Finds the closest asteroid to the given position. Assigns that asteroids
	// EntityID to targetID. If no asteroid is found, targetID is assigned
	// ENTITY_ID_NULL
	b2Vec2 FindTarget(const b2Vec2& pos, EntityID source, EntityID& targetID) const;
};