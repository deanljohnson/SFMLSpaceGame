#pragma once

class Gun 
{
public:
	virtual ~Gun()
	{
	}

	virtual void Shoot() = 0;
	virtual float GetNormalizedHeat() = 0;
};