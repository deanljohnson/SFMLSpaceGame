#pragma once

class Gun 
{
public:
	virtual ~Gun()
	{
	}

	// Fires the Gun. Optionally, supply a position to fire at.
	// Not all gun types will use that position in a meaningful way.
	virtual void Shoot(const b2Vec2& pos = b2Vec2()) = 0;
	virtual float GetNormalizedHeat() = 0;
};