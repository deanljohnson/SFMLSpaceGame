// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/KeyListener.h>
#include <GameState.h>

KeyListener::KeyListener(EntityID ent, std::set<sf::Keyboard::Key> keys)
	: Component(ent),
	  m_keys(keys),
	  m_callbacks()
{
}

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

void KeyListener::operator+=(Callback callback)
{
	m_callbacks.push_back(callback);
}