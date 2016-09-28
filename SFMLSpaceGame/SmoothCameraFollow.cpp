#include "SmoothCameraFollow.h"
#include "Entity.h"
#include "WorldConstants.h"

void SmoothCameraFollow::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_LastPosition = m_position->position;
}

void SmoothCameraFollow::Update()
{
	auto v = GAME_WINDOW->getView();
	v.setCenter(MapWorldToSFML(m_LastPosition));
	GAME_WINDOW->setView(v);

	m_LastPosition = m_position->position;
}