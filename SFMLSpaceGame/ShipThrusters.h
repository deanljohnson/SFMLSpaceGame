#pragma once
#include "Physics.h"

enum ThrustDirection
{
	Front, Left, Right, Reverse
};

struct ShipThrust
{
	ShipThrust(float f, float s, float r)
		: Forward(f), Side(s), Reverse(r) {}
	float Forward, Side, Reverse;

	b2Vec2 ShipThrust::Get(ThrustDirection dir);
};

class ShipThrusters : public Component
{
private:
	Physics* m_physics{ nullptr };
	ShipThrust m_strength;
	ShipThrust m_maxStrength;

public:
	ShipThrusters() 
		: m_strength(.2f,.2f,.2f), 
		  m_maxStrength(5.f,5.f,5.f)
	{};

	void ApplyThrust(ThrustDirection dir);
	virtual void Init() override;
};