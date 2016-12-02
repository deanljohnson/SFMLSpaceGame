// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/SmoothCameraFollow.h>
#include <Components/Position.h>
#include <Entity.h>
#include <WorldConstants.h>

SmoothCameraFollow::SmoothCameraFollow(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>())
{
	m_lastPosition = m_position.position;
}

void SmoothCameraFollow::Update()
{
	auto v = GAME_WINDOW->getView();
	v.setCenter(MapWorldToPixel(m_lastPosition));
	GAME_WINDOW->setView(v);

	m_lastPosition = m_position.position;
}