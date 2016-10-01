#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <WorldConstants.h>

class CirclePrimitive : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	sf::CircleShape m_shape;

public:
	explicit CirclePrimitive(float rad) : m_shape(rad * PIXELS_PER_METER){}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};