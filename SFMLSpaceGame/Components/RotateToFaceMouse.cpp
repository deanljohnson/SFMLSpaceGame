#include <Components/RotateToFaceMouse.h>
#include <SFML/Window/Mouse.hpp>
#include <WorldConstants.h>
#include <Entity.h>
#include <Box2D/Common/b2Math.h>
#include <Components/Rotation.h>

#ifndef M_TAU
#define M_TAU (float)(M_PI + M_PI)
#endif

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

	// figure where our current angular velocity is taking us
	float nextAngle = m_physics->GetRotationRadians() 
						+ (m_physics->GetAngularVelocity() / (3.f * m_smoothingScale));
	float totalRotation = targetAngle - nextAngle;

	// constrain rotation to (-M_PI, M_PI)
	while (totalRotation < -M_PI)
		totalRotation += M_TAU;
	while (totalRotation > M_PI)
		totalRotation -= M_TAU;

	float lerpFactor = abs(totalRotation) > 3.f
		? 1.f
		: abs(totalRotation) / 3.f;

	if (totalRotation < -0.01f)
		m_thrusters->ApplyThrust(SteerLeft, lerpFactor);
	else if (totalRotation > 0.01f)
		m_thrusters->ApplyThrust(SteerRight, lerpFactor);
}