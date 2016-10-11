#pragma once
#include <Components/Physics.h>
#include "ShipThrusters.h"

class RotateToFaceMouse : public Component
{
private:
	Physics* m_physics{ nullptr };
	ShipThrusters* m_thrusters{ nullptr };
	float m_smoothingScale;

public:
	explicit RotateToFaceMouse(float smoothingScale)
		: m_smoothingScale(smoothingScale)
	{}

	virtual void Init() override;
	virtual void Update() override;
};