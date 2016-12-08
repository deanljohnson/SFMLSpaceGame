// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <BatchIndex.h>
#include <RenderBatch.h>
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\VertexArray.hpp>

BatchIndex::BatchIndex(size_t indx, size_t vertIdx, RenderBatch& batch)
	: index(indx), vertCount(4), vertIndex(vertIdx), m_batch(batch)
{}

void BatchIndex::Move(float x, float y) 
{
	m_batch.Move(this, x, y);
}

void BatchIndex::Move(const sf::Vector2f& amt)
{
	m_batch.Move(this, amt);
}

void BatchIndex::SetPosition(const sf::Vector2f& pos)
{
	m_batch.SetPosition(this, pos);
}

sf::Vector2f BatchIndex::GetPosition()
{
	return m_batch.GetPosition(this);
}

void BatchIndex::SetOrigin(const sf::Vector2f& origin)
{
	m_batch.SetOrigin(this, origin);
}

sf::Vector2f BatchIndex::GetOrigin()
{
	return m_batch.GetOrigin(this);
}

sf::Vector2f BatchIndex::GetScaledOrigin()
{
	return m_batch.GetScaledOrigin(this);
}

void BatchIndex::SetRotation(float rot)
{
	m_batch.SetRotation(this, rot);
}

float BatchIndex::GetRotation()
{
	return m_batch.GetRotation(this);
}

void BatchIndex::SetScale(const sf::Vector2f& scale)
{
	m_batch.SetScale(this, scale);
}

sf::Vector2f BatchIndex::GetScale()
{
	return m_batch.GetScale(this);
}

void BatchIndex::SetRect(const sf::IntRect& rect)
{
#ifdef _DEBUG
	if (vertCount != 4)
		printf("Setting the rect of a BatchIndex with %d vertices is most likely an error", vertCount);
#endif
	m_batch.SetRect(this, rect);
}

sf::IntRect BatchIndex::GetRect()
{
	return m_batch.GetRect(this);
}

void BatchIndex::SetColor(const sf::Color& color)
{
	m_batch.SetColor(this, color);
}

sf::Color BatchIndex::GetColor()
{
	return m_batch.GetColor(this);
}

sf::Texture* BatchIndex::GetTexture() 
{
	return m_batch.GetTexture();
}

void BatchIndex::Remove()
{
	m_batch.Remove(this);
}

sf::Vertex& BatchIndex::operator[](size_t index) 
{
	return m_batch.GetVertex(this, index);
}