#include <Components/RotateToFaceMouse.h>
#include <SFML/Window/Mouse.hpp>
#include <WorldConstants.h>
#include <Entity.h>
#include <Box2D/Common/b2Math.h>
#include "ExtendedMath.h"

b2Vec2 GetWorldMouseLocation() 
{
	auto sfPos = GAME_WINDOW->mapPixelToCoords(sf::Mouse::getPosition(*GAME_WINDOW));
	return MapPixelToWorld(sfPos);
}

void RotateToFaceMouse::Init() 
{
	m_physics = &entity->GetComponent<Physics>();
	m_thrusters = &entity->GetComponent<ShipThrusters>();
}

void RotateToFaceMouse::Update() 
{
	b2Vec2 mousePos = GetWorldMouseLocation();

	b2Vec2 difVector = mousePos - m_physics->GetPosition();
	float targetAngle = atan2f(difVector.y, difVector.x);

	m_thrusters->SteerTowardsAngle(targetAngle, m_smoothingScale);
}