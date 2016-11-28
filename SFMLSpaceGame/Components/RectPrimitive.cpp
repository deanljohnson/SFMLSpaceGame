#include "stdafx.h"
#include <Components/RectPrimitive.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>

RectPrimitive::RectPrimitive(EntityID ent, float w, float h)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_shape({w,h})
{
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_shape.getSize() / 2.f);

	m_shape.setPosition(B2VecToSFMLVec(m_position.position));
	m_shape.setRotation(m_rotation.GetDegrees());
}

RectPrimitive::RectPrimitive(EntityID ent, const sf::Vector2f& size)
	: RectPrimitive(ent, size.x, size.y)
{
}

void RectPrimitive::Update() 
{
	m_shape.setPosition(B2VecToSFMLVec(m_position.position));
	m_shape.setRotation(m_rotation.GetDegrees());
}
// TODO: Convert this to use render batching
void RectPrimitive::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	DRAW_CALLS++;
	target.draw(m_shape, states);
}