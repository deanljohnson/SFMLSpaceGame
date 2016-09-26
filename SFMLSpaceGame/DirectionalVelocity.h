#pragma once
#include "Component.h"
#include "Physics.h"
#include "DirectionalKeyboardInput.h"

class DirectionalVelocity : public Component
{
private:
	DirectionalKeyboardInput* m_input{ nullptr };
	Physics* m_physics{ nullptr };

public:
	virtual void Init() override;
	virtual void Update() override;
};