#pragma once
#include "Position.h"
#include "Rotation.h"
#include <SFML/Graphics/RectangleShape.hpp>

class RectPrimitive : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	sf::RectangleShape m_shape;

public:
	RectPrimitive(float w, float h) : m_shape(sf::Vector2f(w, h)) {}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
	
	const sf::RectangleShape& GetShape() { return m_shape; }
};