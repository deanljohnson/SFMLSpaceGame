#pragma once
#include <Components/Component.h>

class DirectionalKeyboardInput : public Component
{
private:
public:
	explicit DirectionalKeyboardInput(EntityID ent);

	sf::Vector2f input;

	virtual void Update() override;
};