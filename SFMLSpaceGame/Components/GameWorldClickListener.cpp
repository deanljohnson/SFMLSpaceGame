#include "stdafx.h"
#include <Components/GameWorldClickListener.h>
#include <Interfaces/MouseListener.h>
#include <GameState.h>

GameWorldClickListener::GameWorldClickListener(EntityID ent)
	: Component(ent),
	  m_leftClickedState(None),
	  m_rightClickedState(None)
{
}

void GameWorldClickListener::HandleLeftPress()
{
	switch (m_leftClickedState)
	{
	case None:
		m_leftClickedState = Down;
		break;
	case Down:
		m_leftClickedState = Held;
		break;
	case Held:
		break;
	case Click:
		m_leftClickedState = Down;
		break;
	}
}

void GameWorldClickListener::HandleLeftRelease()
{
	switch (m_leftClickedState)
	{
	case None:
		break;
	case Down:
		m_leftClickedState = Click;
		break;
	case Held:
		m_leftClickedState = Click;
		break;
	case Click:
		m_leftClickedState = None;
		break;
	}
}

void GameWorldClickListener::HandleRightPress()
{
	switch (m_rightClickedState)
	{
	case None:
		m_rightClickedState = Down;
		break;
	case Down:
		m_rightClickedState = Held;
		break;
	case Held:
		break;
	case Click:
		m_rightClickedState = Down;
		break;
	}
}

void GameWorldClickListener::HandleRightRelease()
{
	switch (m_rightClickedState)
	{
	case None:
		break;
	case Down:
		m_rightClickedState = Click;
		break;
	case Held:
		m_rightClickedState = Click;
		break;
	case Click:
		m_rightClickedState = None;
		break;
	}
}

void GameWorldClickListener::Update()
{
	for (auto i = 0u; i < GameState::pendingEvents.size(); i++)
	{
		auto& e = GameState::pendingEvents[i];
		bool handled = false;

		if (e->type == sf::Event::MouseButtonPressed)
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				HandleLeftPress();
				handled = true;
			}
			else if (e->mouseButton.button == sf::Mouse::Right)
			{
				HandleRightPress();
				handled = true;
			}
		}
		else if (e->type == sf::Event::MouseButtonReleased)
		{
			if (e->mouseButton.button == sf::Mouse::Left)
			{
				HandleLeftRelease();
				handled = true;
			}
			else if (e->mouseButton.button == sf::Mouse::Right)
			{
				HandleRightRelease();
				handled = true;
			}
		}

		if (handled)
			GameState::pendingEvents.erase(GameState::pendingEvents.begin() + i);
	}

	b2Vec2 mousePos = GetWorldMouseLocation();
	if (m_leftClickedState == Down || m_leftClickedState == Held) 
	{
		for (auto listener : m_leftHeldListeners) 
		{
			listener->OnHeld(mousePos);
		}
	}
	else if (m_leftClickedState == Click) 
	{
		for (auto listener : m_leftClickListeners)
		{
			listener->OnClick(mousePos);
		}
		m_leftClickedState = None;
	}

	if (m_rightClickedState == Down || m_rightClickedState == Held)
	{
		for (auto listener : m_rightClickListeners)
		{
			listener->OnHeld(mousePos);
		}
	}
	else if (m_rightClickedState == Click)
	{
		for (auto listener : m_rightClickListeners)
		{
			listener->OnClick(mousePos);
		}
		m_rightClickedState = None;
	}
}

void GameWorldClickListener::AddClickListener(MouseListener* listener)
{
	switch (listener->button)
	{
	case sf::Mouse::Button::Left:
		m_leftClickListeners.push_back(listener);
		break;
	case sf::Mouse::Button::Right:
		m_rightClickListeners.push_back(listener);
		break;
	default:
		throw "GameWorldClickListener does not support the given mouse button";
	}
	
}

void GameWorldClickListener::AddHeldListener(MouseListener* listener)
{
	switch (listener->button)
	{
	case sf::Mouse::Button::Left:
		m_leftHeldListeners.push_back(listener);
		break;
	case sf::Mouse::Button::Right:
		m_rightHeldListeners.push_back(listener);
		break;
	default:
		throw "GameWorldClickListener does not support the given mouse button";
	}
}