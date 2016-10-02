#pragma once
#include <Components/Physics.h>

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

public:
	ShipThrusters() 
		: m_strength(.3f,.3f,.3f) 
	{};

	explicit ShipThrusters(const ShipThrust& strength)
		: m_strength(strength) 
	{}

	void ApplyThrust(ThrustDirection dir);
	virtual void Init() override;
};