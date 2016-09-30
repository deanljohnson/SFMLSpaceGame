#pragma once
#include <Components/DirectionalKeyboardInput.h>
#include <Components/ShipThrusters.h>

class ThrusterInput : public Component
{
private:
	DirectionalKeyboardInput* m_input{ nullptr };
	ShipThrusters* m_thrusters{ nullptr };
public:
	virtual void Init() override;
	virtual void Update() override;
};
