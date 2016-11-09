#pragma once
#include <Components/DirectionalKeyboardInput.h>
#include <Components/ShipThrusters.h>

class ThrusterInput : public Component, public DefaultSerializeable<ThrusterInput>
{
private:
	DirectionalKeyboardInput& m_input;
	ShipThrusters& m_thrusters;
public:
	explicit ThrusterInput(EntityID ent);
	virtual void Update() override;
};
