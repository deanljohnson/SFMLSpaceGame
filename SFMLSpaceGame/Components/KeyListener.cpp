#include "stdafx.h"
#include <Components/KeyListener.h>
#include <GameState.h>

void KeyListener::Update()
{
	for (auto& e : GameState::pendingEvents)
	{
		// If it's a key event for one of the
		// keys we are listening to
		if (e->type == sf::Event::KeyReleased
			&& m_keys.find(e->key.code) != m_keys.end())
		{
			// It is, call our callbacks
			for (auto& c : m_callbacks)
			{
				c(e->key.code);
			}
		}
	}
}

void KeyListener::operator+=(std::function<void(sf::Keyboard::Key)> callback)
{
	m_callbacks.push_back(callback);
}