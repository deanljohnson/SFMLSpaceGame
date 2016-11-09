#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/RectangleShape.hpp>

class RectPrimitive : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	sf::RectangleShape m_shape;

public:
	RectPrimitive(EntityID ent, float w, float h);

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	
	const sf::RectangleShape& GetShape() const { return m_shape; }
};