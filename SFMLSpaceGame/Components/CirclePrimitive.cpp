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

	// Move the shape the it's current transform values
	m_shape.setPosition(B2VecToSFMLVec(m_position->position));
	m_shape.setRotation(m_rotation->GetDegrees());
}

void CirclePrimitive::Update()
{
	m_shape.setPosition(B2VecToSFMLVec(m_position->position));
	// NOTE: circle rotation is essentially meaningless, don't bother setting it
}

void CirclePrimitive::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(m_shape, states);
}