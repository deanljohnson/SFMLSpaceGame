// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/ThrusterInput.h>
#include <Components/DirectionalKeyboardInput.h>
#include <Components/ShipThrusters.h>
#include <Entity.h>

ThrusterInput::ThrusterInput(EntityID ent)
	: Component(ent),
	m_input(entity->GetComponent<DirectionalKeyboardInput>()),
	m_thrusters(entity->GetComponent<ShipThrusters>())
{
}

void ThrusterInput::Update()
{
	if (m_input.input.x > 0)
		m_thrusters.ApplyThrust(Right);
	if (m_input.input.x < 0)
		m_thrusters.ApplyThrust(Left);
	if (m_input.input.y > 0)
		m_thrusters.ApplyThrust(Reverse);
	if (m_input.input.y < 0)
		m_thrusters.ApplyThrust(Front);
}

