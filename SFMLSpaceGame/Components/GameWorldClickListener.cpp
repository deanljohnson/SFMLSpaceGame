#include <Components/GameWorldClickListener.h>
#include <SFML/Window/Mouse.hpp>

void GameWorldClickListener::Update()
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		switch (m_clickedState)
		{
		case None:
			m_clickedState = None; 
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
	}else
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
