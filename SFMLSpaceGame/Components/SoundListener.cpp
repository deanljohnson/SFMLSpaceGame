// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/SoundListener.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>

SoundListener::SoundListener(EntityID ent)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>())
{
}

void SoundListener::Update()
{
	// NOTE: Spatialization is handled in 3D, and the y-axis
	// corresponds to the 3rd dimension in SFML's audio module
	sf::Listener::setPosition(m_position.X(), 0.f, m_position.Y());
	auto heading = Rotate(b2Vec2(1, 0), m_rotation.GetRadians());
	sf::Listener::setDirection(heading.x, 0.f, heading.y);
}