#include "stdafx.h"
#include <Components/Background.h>
#include <ResourceLoader.h>
#include <Entity.h>
#include <Components/Position.h>
#include <WorldConstants.h>
#include <VectorMath.h>

void Background::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_tex = LoadTexture(m_resourceID);
	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.scale(2.f * METERS_PER_PIXEL, 2.f * METERS_PER_PIXEL); //scale to be in terms of meters - the render function will scale back to pixels
}

void Background::Update()
{
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
}

void Background::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(m_sprite, states);
}
