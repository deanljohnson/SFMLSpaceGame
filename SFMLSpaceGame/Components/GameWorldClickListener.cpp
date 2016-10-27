#include "stdafx.h"
#include <Components/GameWorldClickListener.h>
#include <GameState.h>

void GameWorldClickListener::Update()
{
	for (auto i = 0u; i < GameState::pendingEvents.size(); i++)
	{
		auto& e = GameState::pendingEvents[i];

		if (e->type == sf::Event::MouseButtonPressed 
			&& e->mouseButton.button == sf::Mouse::Left)
		{
			switch (m_clickedState)
			{
			case None:
				m_clickedState = Down;
				break;
			case Down:
				m_clickedState = Held;
				break;
			case Held:
				break;
			case Click:
				m_clickedState = Down;
				break;
			}

			GameState::pendingEvents.erase(GameState::pendingEvents.begin() + i);
		}
		else if (e->type == sf::Event::MouseButtonReleased
				&& e->mouseButton.button == sf::Mouse::Left)
		{
			switch (m_clickedState)
			{
			case None:
				break;
			case Down:
				m_clickedState = Click;
				break;
			case Held:
				m_clickedState = Click;
				break;
			case Click:
				m_clickedState = None;
				break;
			}

			GameState::pendingEvents.erase(GameState::pendingEvents.begin() + i);
		}
	}

	b2Vec2 mousePos;
	if (m_clickedState == Down || m_clickedState == Held) 
	{
		for (auto listener : m_heldListeners) 
		{
			listener->OnHeld(mousePos);
		}
	}
	else if (m_clickedState == Click) 
	{
		for (auto listener : m_clickListeners)
		{
			listener->OnClick(mousePos);
		}
		m_clickedState = None;
	}
}

void GameWorldClickListener::AddClickListener(MouseListener* listener)
{
	m_clickListeners.push_back(listener);
}

void GameWorldClickListener::AddHeldListener(MouseListener* listener)
{
	m_heldListeners.push_back(listener);
}