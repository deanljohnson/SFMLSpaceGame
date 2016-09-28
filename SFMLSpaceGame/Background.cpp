#include "Background.h"
#include <SFML/Graphics/Texture.hpp>
#include "ResourceLoader.h"
#include "Entity.h"
#include "Position.h"
#include "WorldConstants.h"

void Background::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_tex.loadFromImage(LoadImageResource(m_path));
	m_sprite = sf::Sprite(m_tex);
	m_sprite.scale(2.f, 2.f);
}

void Background::Update()
{
	m_sprite.setPosition(m_position->position * PIXELS_PER_METER);
}

void Background::Render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}
