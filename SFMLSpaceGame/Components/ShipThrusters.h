#pragma once
#include <Components/Physics.h>

enum ThrustDirection
{
	Front, Left, Right, Reverse, SteerLeft, SteerRight
};

struct ShipThrust
{
	ShipThrust(float front, float side, float reverse, float steer)
		: Forward(front), Side(side), Reverse(reverse), Steer(steer) {}
	float Forward, Side, Reverse, Steer;

	// Gets the force to apply to a ship for the
	// Forward, Side, or Reverse ThrustDirection's
	b2Vec2 ShipThrust::GetMoveForce(ThrustDirection dir);

	// Gets the torque to apply to a ship for the
	// SteerLeft and SteerRight ThrustDirection's
	float ShipThrust::GetTurningForce(ThrustDirection dir);
};

class ShipThrusters : public Component
{
private:
	Physics* m_physics{ nullptr };
	ShipThrust m_strength;
	b2Vec2 m_currentMoveForce;
	float m_currentTorque;

public:
	explicit ShipThrusters(const ShipThrust& strength)
		: m_strength(strength) 
	{}

	void ApplyThrust(ThrustDirection dir, float amount = 1.f);
	virtual void Init() override;
	virtual void Update() override;
};