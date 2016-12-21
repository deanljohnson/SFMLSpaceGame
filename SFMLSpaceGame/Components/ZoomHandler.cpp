// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/ZoomHandler.h>
#include <GameView.h>
#include <GameState.h>

ZoomHandler::ZoomHandler(EntityID ent)
	: Component(ent)
{
	
}

ZoomHandler::~ZoomHandler()
{
	// Reset the zoom
	GameView::ZoomGameView(1.f / m_totalZoom);
}

void ZoomHandler::Update()
{
	for (auto i = 0u; i < GameState::pendingEvents.size(); i++)
	{
		auto& e = GameState::pendingEvents[i];

		if (e->type == sf::Event::MouseWheelScrolled)
		{
			float delta = e->mouseWheelScroll.delta * ZOOM_SCALE;
			m_zoomFactor -= delta;

			// Limit the zoom. If they zoom too far they flip the entire game world around
			m_zoomFactor = std::max(.2f, m_zoomFactor);

			// The change in zoom factor
			float zoomAmount = (m_zoomFactor / m_lastZoom);
			GameView::ZoomGameView(zoomAmount);

			m_totalZoom *= zoomAmount;
			m_lastZoom = m_zoomFactor;

			GameState::pendingEvents.erase(GameState::pendingEvents.begin() + i);
		}
	}
}
