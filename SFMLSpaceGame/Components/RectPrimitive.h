#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/RectangleShape.hpp>

class RectPrimitive : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	sf::RectangleShape m_shape;

public:
	RectPrimitive(float w, float h) 
		: m_shape(sf::Vector2f(w, h)) 
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	
	const sf::RectangleShape& GetShape() const { return m_shape; }
};