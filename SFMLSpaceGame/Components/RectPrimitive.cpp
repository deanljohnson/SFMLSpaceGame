// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/RectPrimitive.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Entity.h>
#include <VectorMath.h>
#include <RenderBatch.h>
#include <BatchIndex.h>

RectPrimitive::RectPrimitive(EntityID ent, float w, float h)
	: Component(ent),
	  m_position(entity->GetComponent<Position>()),
	  m_rotation(entity->GetComponent<Rotation>()),
	  m_size(w, h)
{
	m_batchIndex = RenderBatch::Get(sf::Quads)->Add();

	sf::RectangleShape rect = sf::RectangleShape({ w, h });
	(*m_batchIndex)[0].position = rect.getPoint(0);
	(*m_batchIndex)[1].position = rect.getPoint(1);
	(*m_batchIndex)[2].position = rect.getPoint(2);
	(*m_batchIndex)[3].position = rect.getPoint(3);

	m_batchIndex->SetScale(sf::Vector2f(1, 1));
	m_batchIndex->SetColor(sf::Color::Green);
	m_batchIndex->SetOrigin({ w / 2.f, h / 2.f });
	m_batchIndex->SetPosition(B2VecToSFMLVec(m_position.position));
	m_batchIndex->SetRotation(m_rotation.GetRadians());
}

RectPrimitive::RectPrimitive(EntityID ent, const sf::Vector2f& size)
	: RectPrimitive(ent, size.x, size.y)
{
}

RectPrimitive::~RectPrimitive()
{
	m_batchIndex->Remove();
}

void RectPrimitive::Update() 
{
	m_batchIndex->SetPosition(B2VecToSFMLVec(m_position.position));
	m_batchIndex->SetRotation(m_rotation.GetRadians());
}