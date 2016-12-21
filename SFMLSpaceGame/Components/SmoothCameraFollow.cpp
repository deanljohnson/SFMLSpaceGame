// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/SmoothCameraFollow.h>
#include <Components/Position.h>
#include <Entity.h>
#include <GameView.h>

SmoothCameraFollow::SmoothCameraFollow(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>())
{
	m_lastPosition = m_position.position;
}

void SmoothCameraFollow::Update()
{
	if (m_lastPosition != m_position.position)
	{
		GameView::SetGameViewCenter(m_lastPosition);
		m_lastPosition = m_position.position;
	}
}