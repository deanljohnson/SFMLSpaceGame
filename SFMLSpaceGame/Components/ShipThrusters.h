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
	ShipThrust* m_strength;
	b2Vec2 m_currentMoveForce;
	float m_currentTorque;

	float m_power; 

public:
	explicit ShipThrusters(ShipThrust* thrust)
		: m_strength(thrust), m_currentTorque(0.f), m_power(1.f)
	{}

	virtual void Init() override;
	virtual void Update() override;

	// Set a multiplier to simulate the thrusters getting less power,
	// or to simulate the ship cruising calmly
	void SetPower(float val);
	float GetPower();

	float GetStrength(ThrustDirection dir);

	void ApplyThrust(ThrustDirection dir, float amount = 1.f);
	void SteerTowardsHeading(b2Vec2 heading, float lookAheadFactor);
	void SteerTowardsAngle(float angle, float lookAheadFactor);
};