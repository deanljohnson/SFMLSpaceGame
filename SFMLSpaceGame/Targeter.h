#pragma once
#include <EntityID.h>

struct b2Vec2;

class Targeter 
{
public:
	virtual ~Targeter()
	{
	}

	virtual b2Vec2 FindTarget(const b2Vec2& pos, EntityID source) const = 0;
};