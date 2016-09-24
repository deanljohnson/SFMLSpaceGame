#include "CirclePrimitive.h"
#include "Entity.h"

void CirclePrimitive::Init()
{
	m_position = &entity->GetComponent<Position>();

	m_shape.setRadius(m_radius);
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_radius, m_radius);
}

void CirclePrimitive::Update()
{
	m_shape.setPosition(m_position->position);
}

void CirclePrimitive::Render(sf::RenderTarget& target)
{
	target.draw(m_shape);
}