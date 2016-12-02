// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/RectPrimitive.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>
#include <RenderBatch.h>

RectPrimitive::RectPrimitive(EntityID ent, float w, float h)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_size(w, h)
{
	m_batch = RenderBatch::Get(sf::Quads);
	m_batchIndex = m_batch->Add();

	m_batch->SetRect(m_batchIndex, { 0, 0, (int)(w * PIXELS_PER_METER), (int)(h * PIXELS_PER_METER) });
	m_batch->SetScale(m_batchIndex, sf::Vector2f(METERS_PER_PIXEL, METERS_PER_PIXEL));
	m_batch->SetColor(m_batchIndex, sf::Color::Green);
	m_batch->SetOrigin(m_batchIndex, { w / 2.f, h / 2.f });
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());

}

RectPrimitive::RectPrimitive(EntityID ent, const sf::Vector2f& size)
	: RectPrimitive(ent, size.x, size.y)
{
}

RectPrimitive::~RectPrimitive()
{
	m_batch->Remove(m_batchIndex);
}

void RectPrimitive::Update() 
{
	m_batch->SetPosition(m_batchIndex, B2VecToSFMLVec(m_position.position));
	m_batch->SetRotation(m_batchIndex, m_rotation.GetRadians());
}