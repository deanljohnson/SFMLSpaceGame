#include <Components/CirclePrimitive.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>

void CirclePrimitive::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());
}

void CirclePrimitive::Update()
{
	//position is stored in meters, must convert to pixels
	m_shape.setPosition(B2VecToSFMLVec(m_position->position));
}

void CirclePrimitive::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(m_shape, states);
}