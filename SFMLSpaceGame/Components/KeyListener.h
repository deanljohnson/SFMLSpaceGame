#pragma once
#include "Component.h"
#include <SFML/Window/Keyboard.hpp>
#include <set>
#include <vector>
#include <functional>
#include <cereal\types\set.hpp>

// Provides a way for entity's to listen
// for certain single key events and call
// any number of callbacks in response
class KeyListener : public Component
{
public:
	typedef std::function<void(sf::Keyboard::Key)> Callback;
private:
	std::set<sf::Keyboard::Key> m_keys;
	std::vector<Callback> m_callbacks;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_keys);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<KeyListener>& construct)
	{
		EntityID selfID;
		std::set<sf::Keyboard::Key> keys;
		ar(selfID, keys);
		construct(selfID, keys);
	}
public:
	KeyListener(EntityID ent, std::set<sf::Keyboard::Key> keys);

	virtual void Update() override;

	void operator +=(Callback callback);
};