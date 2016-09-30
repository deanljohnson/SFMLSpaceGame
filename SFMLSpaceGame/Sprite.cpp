#include "Sprite.h"
#include "Entity.h"
#include "WorldConstants.h"

void Sprite::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
	m_tex = LoadTextureResource(m_resourceID);
	m_sprite = sf::Sprite(*m_tex.get());

	m_sprite.setOrigin(sf::Vector2f(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f));
}

void Sprite::Update()
{
	//position is stored in meters, must convert to pixels
	m_sprite.setPosition(m_position->position * PIXELS_PER_METER);
	m_sprite.setRotation(m_rotation->GetDegrees());
}

void Sprite::Render(sf::RenderTarget& target)
{
	target.draw(m_sprite);
}

sf::IntRect Sprite::GetPixelLocalBounds() const
{
	auto bounds = m_sprite.getLocalBounds();
	bounds.width *= m_sprite.getScale().x;
	bounds.height *= m_sprite.getScale().y;

	return sf::IntRect(bounds.left, bounds.top, bounds.width, bounds.height);
}
