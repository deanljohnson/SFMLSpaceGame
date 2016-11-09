#pragma once
#include <Components/Component.h>
#include <Components/Physics.h>
#include <Components/DirectionalKeyboardInput.h>

class DirectionalVelocity : public Component
{
private:
	DirectionalKeyboardInput& m_input;
	Physics& m_physics;
	float m_maxVelocity;

public:
	DirectionalVelocity(EntityID ent, float maxVel);

	virtual void Update() override;
};