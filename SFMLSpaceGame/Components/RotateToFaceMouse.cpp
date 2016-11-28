#include "stdafx.h"
#include <Components/RotateToFaceMouse.h>
#include <Components/Physics.h>
#include <Components/ShipThrusters.h>
#include <WorldConstants.h>
#include <Entity.h>

RotateToFaceMouse::RotateToFaceMouse(EntityID ent, float smoothingScale)
	: Component(ent), 
	  m_physics(entity->GetComponent<Physics>()),
	  m_thrusters(entity->GetComponent<ShipThrusters>()),
	  m_smoothingScale(smoothingScale)
{
}

void RotateToFaceMouse::Update() 
{
	b2Vec2 mousePos = GetWorldMouseLocation();

	b2Vec2 difVector = mousePos - m_physics.GetPosition();
	float targetAngle = atan2f(difVector.y, difVector.x);

	m_thrusters.SteerTowardsAngle(targetAngle, m_smoothingScale);
}