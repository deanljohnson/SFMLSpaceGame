#pragma once
#include <Box2D\Common\b2Math.h>
#include <EntityID.h>

class Targeter 
{
public:
	virtual b2Vec2 FindTarget(const b2Vec2& pos, EntityID source) const = 0;
};