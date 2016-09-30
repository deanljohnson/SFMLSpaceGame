#include <Components/GameWorldClickListener.h>
#include <SFML/Window/Mouse.hpp>

void GameWorldClickListener::Update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
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
	else
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

	if (m_clickedState == Down || m_clickedState == Held) 
	{
		for (auto listener : m_heldListeners) 
		{
			listener();
		}
	}
	else if (m_clickedState == Click) 
	{
		for (auto listener : m_clickListeners)
		{
			listener();
		}
	}
}

void GameWorldClickListener::AddClickListener(std::function<void()> listener) 
{
	m_clickListeners.push_back(listener);
}

void GameWorldClickListener::AddHeldListener(std::function<void()> listener) 
{
	m_heldListeners.push_back(listener);
}