#include "Background.h"
#include "ResourceLoader.h"
#include "Entity.h"
#include "Position.h"
#include "WorldConstants.h"

void Background::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_tex = LoadTextureResource(m_resourceID);
	m_sprite = sf::Sprite(*m_tex.get());
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
