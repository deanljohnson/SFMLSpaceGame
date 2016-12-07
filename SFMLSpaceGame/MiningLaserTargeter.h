#pragma once
#include <Targeter.h>

class MiningLaserTargeter : public Targeter 
{
public:
	virtual b2Vec2 FindTarget(const b2Vec2& pos, EntityID source) const override;
};