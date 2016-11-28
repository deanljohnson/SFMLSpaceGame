#pragma once
#include <DefaultSerializeable.h>

class DirectionalKeyboardInput : public Component, public DefaultSerializeable<DirectionalKeyboardInput>
{
public:
	explicit DirectionalKeyboardInput(EntityID ent);

	sf::Vector2f input;

	virtual void Update() override;
};