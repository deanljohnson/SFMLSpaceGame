#pragma once
#include <Components/Component.h>
#include <cereal/cereal.hpp>
#include <Components/Interfaces/Updateable.h>

class Physics;

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

class ShipThrusters : public Component, public Updateable
{
private:
	Physics& m_physics;
	std::string m_shipID;
	ShipThrust* m_strength;
	b2Vec2 m_currentMoveForce;
	b2Vec2 m_lastMoveForce;
	float m_currentTorque;

	float m_power; 

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(GetEntityID(), m_shipID, m_currentMoveForce, m_currentTorque, m_power);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShipThrusters>& construct)
	{
		EntityID selfID;
		std::string shipID;
		ar(selfID, shipID);
		construct(selfID, shipID);

		ar(construct->m_currentMoveForce,
			construct->m_currentTorque,
			construct->m_power);
	}

public:
	ShipThrusters(EntityID ent, const std::string& shipID);

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