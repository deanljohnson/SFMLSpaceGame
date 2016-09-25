#pragma once
#include "Component.h"
#include "Position.h"
#include <SFML/Graphics/CircleShape.hpp>

class CirclePrimitive : public Component
{
private:
	Position* m_position{ nullptr };
	sf::CircleShape m_shape;
	float m_radius;

public:
	CirclePrimitive(float rad) : m_radius(rad){}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};