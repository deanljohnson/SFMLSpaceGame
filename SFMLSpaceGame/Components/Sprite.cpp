#include "stdafx.h"
#include <Components/Sprite.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>

Sprite::Sprite(EntityID ent, const std::string& location)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_location(location)
{
	m_batch = RenderBatch::Get(location);
	m_batchIndex = m_batch->Add();
	
	auto rect = m_batch->GetTextureRect(m_batchIndex);

	m_batch->SetScale(m_batchIndex, sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
	m_batch->SetOrigin(m_batchIndex, sf::Vector2f(rect.width / 2.f, rect.height / 2.f));
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());
}

Sprite::~Sprite()
{
	m_batch->Remove(m_batchIndex);
}

void Sprite::Update()
{
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());
}

sf::FloatRect Sprite::GetDimensions() const
{
	auto rect = m_batch->GetTextureRect(m_batchIndex);
	auto scale = m_batch->GetScale(m_batchIndex);

	return sf::FloatRect(rect.left, rect.top, rect.width * scale.x, rect.height * scale.y);
}

b2Vec2 Sprite::GetOrigin() const
{
	return SFMLVecToB2Vec(m_batch->GetScaledOrigin(m_batchIndex));
}