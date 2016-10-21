#include <Components/AnimatedSprite.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>
#include <GameTime.h>
#include <SpriteHelpers.h>

AnimatedSprite::AnimatedSprite(ResourceID id, OriginOption origin)
{
	m_animation = LoadAnimationResource(id);
	
	m_batch = RenderBatch::Get(id);
	m_batchIndex = m_batch->Add();

	m_batch->SetTextureRect(m_batchIndex, m_animation->GetCurrentFrame());
	auto rect = m_batch->GetTextureRect(m_batchIndex);

	m_batch->SetScale(m_batchIndex, sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
	m_batch->SetOrigin(m_batchIndex, SpriteHelpers::GetOrigin(rect, origin));
}

void AnimatedSprite::Init()
{
	m_position = &entity->GetComponent<Position>();
	m_rotation = &entity->GetComponent<Rotation>();

	m_offset = b2Vec2(0, 0);

	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position->position + Rotate(m_offset, m_rotation->GetRadians())));
	m_batch->SetRotation(m_batchIndex, m_rotation->GetRadians());
}

void AnimatedSprite::Update()
{
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position->position + Rotate(m_offset, m_rotation->GetRadians())));
	m_batch->SetRotation(m_batchIndex, m_rotation->GetRadians());
	m_animation->Update(GameTime::deltaTime);
	m_batch->SetTextureRect(m_batchIndex, m_animation->GetCurrentFrame());

	if (next != nullptr) next->Update();
}

void AnimatedSprite::SetOffset(const b2Vec2& v)
{
	m_offset = v;
}

void AnimatedSprite::SetScale(float x, float y)
{
	m_batch->SetScale(m_batchIndex, {(x * METERS_PER_PIXEL), (y * METERS_PER_PIXEL) });
}

sf::FloatRect AnimatedSprite::GetDimensions() const
{
	auto rect = m_batch->GetTextureRect(m_batchIndex);
	auto scale = m_batch->GetScale(m_batchIndex);

	return sf::FloatRect(rect.left, rect.top, rect.width * scale.x, rect.height * scale.y);
}