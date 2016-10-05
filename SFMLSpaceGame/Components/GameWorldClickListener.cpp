#include <Components/GameWorldClickListener.h>
#include <SFML/Window/Mouse.hpp>
#include <GameState.h>

void GameWorldClickListener::Update()
{
	for (auto& e : GameState::pendingEvents)
	{
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