#include "stdafx.h"
#include <Components/Sprite.h>
#include <SpriteHelpers.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <RenderBatch.h>

Sprite::Sprite(EntityID ent, const std::string& id, OriginOption origin)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_offset(b2Vec2(0,0)),
	  m_id(id),
	  m_originOption(origin)
{
	m_batch = RenderBatch::Get(id);
	m_batchIndex = m_batch->Add();

	auto rect = m_batch->GetRect(m_batchIndex);

	m_batch->SetScale(m_batchIndex, sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
	m_batch->SetOrigin(m_batchIndex, SpriteHelpers::GetOrigin(rect, origin));

	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position + Rotate(m_offset, m_rotation.GetRadians())));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());
}

Sprite::~Sprite()
{
	m_batch->Remove(m_batchIndex);
}

void Sprite::Update()
{
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position + Rotate(m_offset, m_rotation.GetRadians())));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());

	if (next != nullptr) next->Update();
}

void Sprite::SetOffset(const b2Vec2& v)
{
	m_offset = v;
}

void Sprite::SetScale(float x, float y)
{
	m_batch->SetScale(m_batchIndex, { (x * METERS_PER_PIXEL), (y * METERS_PER_PIXEL) });
}

void Sprite::SetTextureRect(const sf::IntRect& rect) 
{
	m_batch->SetRect(m_batchIndex, rect);

	// if we do not re-set the origin, changing texture rect's
	// can cause distortion issues.
	m_batch->SetOrigin(m_batchIndex, SpriteHelpers::GetOrigin(rect, m_originOption));
}

sf::FloatRect Sprite::GetDimensions() const
{
	auto rect = m_batch->GetRect(m_batchIndex);
	auto scale = m_batch->GetScale(m_batchIndex);

	return sf::FloatRect(rect.left, rect.top, rect.width * scale.x, rect.height * scale.y);
}

b2Vec2 Sprite::GetOrigin() const
{
	return SFMLVecToB2Vec(m_batch->GetScaledOrigin(m_batchIndex));
}