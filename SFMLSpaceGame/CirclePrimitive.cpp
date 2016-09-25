#include "CirclePrimitive.h"
#include "Entity.h"
#include "WorldConstants.h"

void CirclePrimitive::Init()
{
	m_position = &entity->GetComponent<Position>();

	m_shape.setRadius(m_radius);
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_radius, m_radius);
}

void CirclePrimitive::Update()
{
	//position is stored in meters, must convert to pixels
	m_shape.setPosition(m_position->position * PIXELS_PER_METER);
}

void CirclePrimitive::Render(sf::RenderTarget& target)
{
	target.draw(m_shape);
}