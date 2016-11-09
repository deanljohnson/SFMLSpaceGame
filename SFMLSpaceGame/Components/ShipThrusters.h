#pragma once
#include <Components/Physics.h>
#include <cereal\cereal.hpp>

enum ThrustDirection
{
	Front, Left, Right, Reverse, SteerLeft, SteerRight
};

struct ShipThrust
{
	ShipThrust() 
		: Forward(0), Side(0), Reverse(0), Steer(0)
	{}
	ShipThrust(float front, float side, float reverse, float steer)
		: Forward(front), Side(side), Reverse(reverse), Steer(steer) {}
	float Forward, Side, Reverse, Steer;

	// Gets the force to apply to a ship for the
	// Forward, Side, or Reverse ThrustDirection's
	b2Vec2 ShipThrust::GetMoveForce(ThrustDirection dir);

	// Gets the torque to apply to a ship for the
	// SteerLeft and SteerRight ThrustDirection's
	float ShipThrust::GetTurningForce(ThrustDirection dir);

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Forward", Forward),
			cereal::make_nvp("Side", Side),
			cereal::make_nvp("Reverse", Reverse),
			cereal::make_nvp("Steer", Steer));
	}
};

class ShipThrusters : public Component
{
private:
	Physics& m_physics;
	ShipThrust* m_strength;
	b2Vec2 m_currentMoveForce;
	b2Vec2 m_lastMoveForce;
	float m_currentTorque;

	float m_power; 

public:
	ShipThrusters(EntityID ent, ShipThrust* thrust);

	virtual void Update() override;

	// Set a multiplier to simulate the thrusters getting less power,
	// or to simulate the ship cruising calmly
	void SetPower(float val);
	float GetPower();

	float GetStrength(ThrustDirection dir);

	b2Vec2 GetCurrentForce() const;

	void ApplyThrust(ThrustDirection dir, float amount = 1.f);
	void SteerTowardsHeading(b2Vec2 heading, float lookAheadFactor);
	void SteerTowardsAngle(float angle, float lookAheadFactor);
};