#pragma once
#include "Physics.h"

class RotateToFaceMouse : public Component
{
private:
	Physics* m_physics{ nullptr };
	float m_torqueScale;
	float m_smoothingScale;

public:
	RotateToFaceMouse(float torqueScale, float smoothingScale)
		: m_torqueScale(torqueScale), 
		  m_smoothingScale(smoothingScale)
	{}

	virtual void Init() override;
	virtual void Update() override;
};