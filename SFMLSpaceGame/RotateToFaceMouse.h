#pragma once
#include "Physics.h"

class RotateToFaceMouse : public Component
{
private:
	Physics* m_physics{ nullptr };
public:
	virtual void Init() override;
	virtual void Update() override;
};