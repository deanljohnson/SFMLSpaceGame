#pragma once
#include "Component.h"
#include "Position.h"

class Physics : Component
{
private:
	Position* m_position{ nullptr };
public:
	Physics() : velocity(sf::Vector2f(0.f, 0.f)){}

	sf::Vector2f velocity;

	virtual void Init() override;
	virtual void Update() override;
};