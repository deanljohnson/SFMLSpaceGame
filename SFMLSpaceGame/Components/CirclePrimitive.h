#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/CircleShape.hpp>

class CirclePrimitive : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	sf::CircleShape m_shape;

public:
	CirclePrimitive(EntityID ent, float rad);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};