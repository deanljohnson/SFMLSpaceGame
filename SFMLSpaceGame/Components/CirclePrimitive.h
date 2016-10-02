#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/CircleShape.hpp>

class CirclePrimitive : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	sf::CircleShape m_shape;

public:
	explicit CirclePrimitive(float rad) : m_shape(rad){}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates& states) override;
};