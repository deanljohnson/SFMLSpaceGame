#include "RotateToFaceMouse.h"
#include <SFML/Window/Mouse.hpp>
#include "WorldConstants.h"
#include "Entity.h"
#include "VectorMath.h"
#include <Box2D/Common/b2Math.h>

b2Vec2 GetWorldMouseLocation() 
{
	auto sfPos = GAME_WINDOW->mapPixelToCoords(sf::Mouse::getPosition(*GAME_WINDOW));
	return MapSFMLToWorld(sfPos);
}

void RotateToFaceMouse::Init() 
{
	m_physics = &entity->GetComponent<Physics>();
}

void RotateToFaceMouse::Update() 
{
	auto mousePos = GetWorldMouseLocation();

	m_physics->RotateTowards(mousePos, 2.5f, 1.f);
}