#include <Components/RectPrimitive.h>
#include <Entity.h>
#include <WorldConstants.h>

void RectPrimitive::Init() 
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void RectPrimitive::Update() 
{
	//position is stored in meters, must convert to pixels
	m_shape.setPosition(m_position->position * PIXELS_PER_METER);
	m_shape.setRotation(m_rotation->GetDegrees());
}

void RectPrimitive::Render(sf::RenderTarget& target)
{
	target.draw(m_shape);
}