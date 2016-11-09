#pragma once
#include <Components/Physics.h>
#include "ShipThrusters.h"

class RotateToFaceMouse : public Component
{
private:
	Physics& m_physics;
	ShipThrusters& m_thrusters;
	float m_smoothingScale;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_smoothingScale);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<RotateToFaceMouse>& construct)
	{
		EntityID selfID;
		float smoothScale;
		ar(selfID, smoothScale);
		construct(selfID, smoothScale);
	}

public:
	RotateToFaceMouse(EntityID ent, float smoothingScale);

	virtual void Update() override;
};