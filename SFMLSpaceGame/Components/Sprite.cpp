#include <Components/Sprite.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>

Sprite::Sprite(ResourceID id)
	: m_tex(LoadBuiltInTexture(id))
{
}

Sprite::Sprite(const std::string& location)
	: m_tex(LoadTexture(location))
{
}

void Sprite::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.setScale(METERS_PER_PIXEL, METERS_PER_PIXEL); //Render method will convert meters back to pixels

	m_sprite.setOrigin(sf::Vector2f(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f));
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
	m_sprite.setRotation(m_rotation->GetDegrees());
}

void Sprite::Update()
{
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position));
	m_sprite.setRotation(m_rotation->GetDegrees());
}

void Sprite::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(m_sprite, states);
}

sf::FloatRect Sprite::GetDimensions() const
{
	auto bounds = m_sprite.getLocalBounds();
	bounds.width *= m_sprite.getScale().x;
	bounds.height *= m_sprite.getScale().y;

	return sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height);
}

b2Vec2 Sprite::GetOrigin() const
{
	return SFMLVecToB2Vec(m_sprite.getOrigin() * METERS_PER_PIXEL);
}