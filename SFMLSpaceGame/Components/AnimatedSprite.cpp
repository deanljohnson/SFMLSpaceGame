#include <Components/AnimatedSprite.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>
#include <GameTime.h>
#include <SpriteHelpers.h>

void AnimatedSprite::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();
	m_animation = LoadAnimationResource(m_resourceID);

	m_sprite = sf::Sprite(*m_animation->GetTexture());
	m_sprite.setScale(METERS_PER_PIXEL, METERS_PER_PIXEL); //Render method will convert meters back to pixels
	m_sprite.setTextureRect(m_animation->GetCurrentFrame());

	SpriteHelpers::SetOrigin(&m_sprite, m_origin);

	m_sprite.setPosition(B2VecToSFMLVec(m_position->position + Rotate(m_offset, m_rotation->GetRadians())));
	m_sprite.setRotation(m_rotation->GetDegrees());

	m_offset = b2Vec2(0, 0);
}

void AnimatedSprite::Update()
{
	m_sprite.setPosition(B2VecToSFMLVec(m_position->position + Rotate(m_offset, m_rotation->GetRadians())));
	m_sprite.setRotation(m_rotation->GetDegrees());

	m_animation->Update(GameTime::deltaTime);
	m_sprite.setTextureRect(m_animation->GetCurrentFrame());

	if (next != nullptr) next->Update();
}

void AnimatedSprite::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(m_sprite, states);

	if (next != nullptr) next->Render(target, states);
}

void AnimatedSprite::SetOffset(const b2Vec2& v)
{
	m_offset = v;
}

void AnimatedSprite::SetScale(float x, float y)
{
	m_sprite.setScale(x * METERS_PER_PIXEL, y * METERS_PER_PIXEL);
}

sf::FloatRect AnimatedSprite::GetDimensions() const
{
	auto bounds = m_sprite.getLocalBounds();
	bounds.width *= m_sprite.getScale().x;
	bounds.height *= m_sprite.getScale().y;

	return sf::FloatRect(bounds.left, bounds.top, bounds.width, bounds.height);
}