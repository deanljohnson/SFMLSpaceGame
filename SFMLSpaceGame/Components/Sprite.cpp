// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/Sprite.h>
#include <SpriteHelpers.h>
#include <Entity.h>
#include <WorldConstants.h>
#include <VectorMath.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <RenderBatch.h>
#include <BatchIndex.h>
#include <AnimationDefinition.h>

void Sprite::InitializeBatchRender()
{
	m_batchIndex = RenderBatch::Get(m_key.texID)->Add();

	if (m_key.type == SpriteKey::Type::TexIndex)
	{
		auto anim = LoadAnimation(m_key.texID);
		SetTextureRect(anim->GetFrame(m_key.index));
	}

	auto rect = m_batchIndex->GetRect();

	m_batchIndex->SetScale(sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
	m_batchIndex->SetOrigin(SpriteHelpers::GetOrigin(rect, m_originOption));

	m_batchIndex->SetPosition(B2VecToSFMLVec(m_position.position + Rotate(m_offset, m_rotation.GetRadians())));
	m_batchIndex->SetRotation(m_rotation.GetRadians());
}

Sprite::Sprite(EntityID ent, const SpriteKey& key, OriginOption origin)
	: Component(ent),
	m_position(entity->GetComponent<Position>()),
	m_rotation(entity->GetComponent<Rotation>()),
	m_offset(b2Vec2(0, 0)),
	m_key(key),
	m_originOption(origin)
{
	InitializeBatchRender();
}

Sprite::Sprite(EntityID ent, const std::string& id, OriginOption origin)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_offset(b2Vec2(0,0)),
	  m_key(id),
	  m_originOption(origin)
{
	InitializeBatchRender();
}

Sprite::~Sprite()
{
	m_batchIndex->Remove();
}

void Sprite::Update()
{
	m_batchIndex->SetPosition(B2VecToSFMLVec(m_position.position + Rotate(m_offset, m_rotation.GetRadians())));
	m_batchIndex->SetRotation(m_rotation.GetRadians());

	if (next != nullptr) next->Update();
}

void Sprite::SetOffset(const b2Vec2& v)
{
	m_offset = v;
}

void Sprite::SetScale(float x, float y)
{
	m_batchIndex->SetScale({ (x * METERS_PER_PIXEL), (y * METERS_PER_PIXEL) });
}

void Sprite::SetTextureRect(const sf::IntRect& rect) 
{
	m_batchIndex->SetRect(rect);

	(*m_batchIndex)[0].position = { 0.f, 0.f };
	(*m_batchIndex)[1].position = { (float)rect.width, 0.f };
	(*m_batchIndex)[2].position = { (float)rect.width, (float)rect.height };
	(*m_batchIndex)[3].position = { 0.f, (float)rect.height };

	// if we do not re-set the origin, changing texture rect's
	// can cause distortion issues.
	m_batchIndex->SetOrigin(SpriteHelpers::GetOrigin(rect, m_originOption));
}

sf::FloatRect Sprite::GetDimensions() const
{
	auto rect = m_batchIndex->GetRect();
	auto scale = m_batchIndex->GetScale();

	return sf::FloatRect(rect.left, rect.top, rect.width * scale.x, rect.height * scale.y);
}

b2Vec2 Sprite::GetOrigin() const
{
	return SFMLVecToB2Vec(m_batchIndex->GetScaledOrigin());
}