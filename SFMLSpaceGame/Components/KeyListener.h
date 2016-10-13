#pragma once
#include "Component.h"
#include <SFML/Window/Keyboard.hpp>
#include <set>
#include <vector>
#include <functional>

class KeyListener : public Component
{
private:
	std::set<sf::Keyboard::Key> m_keys;
	std::vector<std::function<void(sf::Keyboard::Key)>> m_callbacks;
public:
	KeyListener(std::initializer_list<sf::Keyboard::Key> keys)
		: m_keys(keys), m_callbacks()
	{}

	virtual void Update() override;
	void AddCallback(std::function<void(sf::Keyboard::Key)> callback);
};