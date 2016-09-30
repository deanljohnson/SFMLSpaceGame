#pragma once
#include <Components/Component.h>
#include <Components/Physics.h>
#include <Components/DirectionalKeyboardInput.h>

class DirectionalVelocity : public Component
{
private:
	DirectionalKeyboardInput* m_input{ nullptr };
	Physics* m_physics{ nullptr };
	float m_maxVelocity;

public:
	DirectionalVelocity(float maxVel) : m_maxVelocity(maxVel) {}

	virtual void Init() override;
	virtual void Update() override;
};