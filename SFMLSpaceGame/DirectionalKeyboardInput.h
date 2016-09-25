#pragma once
#include "Component.h"

class DirectionalKeyboardInput : public Component
{
private:
public:
	sf::Vector2f input;

	virtual void Update() override;
};