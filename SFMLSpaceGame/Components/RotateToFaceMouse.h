#pragma once
#include <Components/Physics.h>
#include "ShipThrusters.h"

class RotateToFaceMouse : public Component
{
private:
	Physics& m_physics;
	ShipThrusters& m_thrusters;
	float m_smoothingScale;

public:
	RotateToFaceMouse(EntityID ent, float smoothingScale);

	virtual void Update() override;
};