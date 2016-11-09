#pragma once
#include "Component.h"
#include <SFML/Window/Keyboard.hpp>
#include <set>
#include <vector>
#include <functional>

// Provides a way for entity's to listen
// for certain single key events and call
// any number of callbacks in response
class KeyListener : public Component
{
private:
	std::set<sf::Keyboard::Key> m_keys;
	std::vector<std::function<void(sf::Keyboard::Key)>> m_callbacks;
public:
	KeyListener(EntityID ent, std::initializer_list<sf::Keyboard::Key> keys);

	virtual void Update() override;

	void operator +=(std::function<void(sf::Keyboard::Key)> callback);
};