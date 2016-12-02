// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/CirclePrimitive.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>

CirclePrimitive::CirclePrimitive(EntityID ent, float rad)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_shape(rad)
{
	m_shape.setFillColor(sf::Color::Green);
	m_shape.setOrigin(m_shape.getRadius(), m_shape.getRadius());

	// Move the shape the it's current transform values
	m_shape.setPosition(B2VecToSFMLVec(m_position.position));
	m_shape.setRotation(m_rotation.GetDegrees());
}

void CirclePrimitive::Update()
{
	m_shape.setPosition(B2VecToSFMLVec(m_position.position));
	// NOTE: circle rotation is essentially meaningless, don't bother setting it
}

void CirclePrimitive::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(m_shape, states);
}