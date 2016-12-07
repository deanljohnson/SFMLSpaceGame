// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include "stdafx.h"
#include <RenderBatch.h>
#include <BatchIndex.h>
#include <VectorMath.h>
#include <ResourceLoader.h>

std::unordered_map<std::string, std::unique_ptr<RenderBatch>> RenderBatch::m_stringBatches;
std::unordered_map<sf::PrimitiveType, std::unique_ptr<RenderBatch>> RenderBatch::m_primitiveBatches;

RenderBatch* RenderBatch::Get(const std::string& texName)
{
	auto it = m_stringBatches.find(texName);
	if (it != m_stringBatches.end())
		return it->second.get();

	auto batch = std::make_unique<RenderBatch>(LoadTexture(texName));
	
	auto inserted = m_stringBatches.insert(make_pair(texName, move(batch)));

	return inserted.first->second.get();
}

RenderBatch* RenderBatch::Get(sf::PrimitiveType primType)
{
	auto it = m_primitiveBatches.find(primType);
	if (it != m_primitiveBatches.end())
		return it->second.get();

	auto batch = std::make_unique<RenderBatch>(primType);

	auto inserted = m_primitiveBatches.insert(make_pair(primType, move(batch)));

	return inserted.first->second.get();
}

void RenderBatch::RenderAll(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& b : m_stringBatches) 
	{
		b.second->Render(target, states);
	}

	for (auto& b : m_primitiveBatches)
	{
		b.second->Render(target, states);
	}
}

RenderBatch::RenderBatch(std::shared_ptr<sf::Texture> tex)
	: m_texture(tex),
	  m_vertices(),
	  m_texRects(),
	  m_positions(),
	  m_scales(),
	  m_origins(),
	  m_rotations(),
	  m_primType(sf::Quads)
{
}

RenderBatch::RenderBatch(sf::PrimitiveType primType)
	: m_texture(nullptr),
	  m_vertices(),
	  m_texRects(),
	  m_positions(),
	  m_scales(),
	  m_origins(),
	  m_rotations(),
	  m_primType(primType)
{
}

BatchIndex* RenderBatch::Add()
{
	auto index = m_indices.size();
	auto vertIndex = m_vertices.size();

	auto texSize = m_texture != nullptr 
				? m_texture->getSize()
				: sf::Vector2u(1, 1);

	m_texRects.push_back(sf::IntRect(0, 0, static_cast<int>(texSize.x), static_cast<int>(texSize.y)));
	m_positions.push_back(sf::Vector2f(0, 0));
	m_scales.push_back(sf::Vector2f(1, 1));
	m_origins.push_back(sf::Vector2f(0, 0));
	m_rotations.push_back(b2Rot(0));

	auto vert1 = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White);
	auto vert2 = sf::Vertex(sf::Vector2f(texSize.x, 0), sf::Color::White);
	auto vert3 = sf::Vertex(sf::Vector2f(texSize.x, texSize.y), sf::Color::White);
	auto vert4 = sf::Vertex(sf::Vector2f(0, texSize.y), sf::Color::White);

	vert1.texCoords = sf::Vector2f(0,0);
	vert2.texCoords = sf::Vector2f(texSize.x,0);
	vert3.texCoords = sf::Vector2f(texSize.x, texSize.y);
	vert4.texCoords = sf::Vector2f(0, texSize.y);

	m_vertices.push_back(vert1);
	m_vertices.push_back(vert2);
	m_vertices.push_back(vert3);
	m_vertices.push_back(vert4);

	BatchIndex* i = new BatchIndex(index, vertIndex, *this);
	m_indices.push_back(std::unique_ptr<BatchIndex>(i));

	return i;
}

void RenderBatch::Remove(BatchIndex* index)
{
	m_removedIndices.push_back(index);
}

void RenderBatch::Move(BatchIndex* index, float x, float y)
{
	unsigned int limit = (index->vertIndex) + index->vertCount;
	for (unsigned int i = index->vertIndex; i < limit; i++)
	{
		m_vertices[i].position.x += x;
		m_vertices[i].position.y += y;
	}

	m_positions[index->index].x += x;
	m_positions[index->index].y += y;
}

void RenderBatch::Move(BatchIndex* index, const sf::Vector2f& amt)
{
	Move(index, amt.x, amt.y);
}

void RenderBatch::SetPosition(BatchIndex* index, const sf::Vector2f& pos)
{
	Move(index, pos - (m_positions[index->index] + Rotate(MultComponents(m_origins[index->index], m_scales[index->index]),
													m_rotations[index->index])));
}

sf::Vector2f RenderBatch::GetPosition(BatchIndex* index)
{
	sf::Vector2f origin = Rotate(MultComponents(m_origins[index->index], m_scales[index->index]),
									m_rotations[index->index]);

	return m_positions[index->index] + origin;
}

void RenderBatch::SetOrigin(BatchIndex* index, const sf::Vector2f& origin)
{
	sf::Vector2f originDif = origin - m_origins[index->index];
	originDif = Rotate(MultComponents(originDif, m_scales[index->index]), m_rotations[index->index]);
	Move(index, -originDif);
	m_origins[index->index] = origin;
}

sf::Vector2f RenderBatch::GetOrigin(BatchIndex* index)
{
	return m_origins[index->index];
}

sf::Vector2f RenderBatch::GetScaledOrigin(BatchIndex* index)
{
	return MultComponents(m_origins[index->index], m_scales[index->index]);
}

void RenderBatch::SetRotation(BatchIndex* index, float ang)
{
	auto originalRot = m_rotations[index->index];
	m_rotations[index->index] = b2Rot(ang);

	// Determine the amount to rotate by
	float ang_t = ang - originalRot.GetAngle();

	sf::Vector2f originScaled = GetScaledOrigin(index);
	originScaled = Rotate(originScaled, originalRot) + m_positions[index->index];

	// at this point, originScaled is the origin of our quad
	float cosT = cosf(ang_t);
	float sinT = sinf(ang_t);

	for (unsigned i = index->vertIndex; i < index->vertIndex + index->vertCount; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originScaled;
		m_vertices[i].position = sf::Vector2f((cosT * dif.x) - (sinT * dif.y) + originScaled.x,
											(sinT * dif.x) + (cosT * dif.y) + originScaled.y);
	}

	m_positions[index->index] = m_vertices[index->vertIndex].position;
}

float RenderBatch::GetRotation(BatchIndex* index)
{
	return m_rotations[index->index].GetAngle();
}

void RenderBatch::SetScale(BatchIndex* index, const sf::Vector2f& scale)
{
 	const auto originalScale = m_scales[index->index];
	m_scales[index->index] = scale;

	const sf::Vector2f originalSize(m_texRects[index->index].width * originalScale.x, m_texRects[index->index].height * originalScale.y);
	const sf::Vector2f targetSize(m_texRects[index->index].width * scale.x, m_texRects[index->index].height * scale.y);
	const sf::Vector2f sizeDif = targetSize - originalSize;

	const auto leftOriginFactor = m_origins[index->index].x / m_texRects[index->index].width;
	const auto rightOriginFactor = 1.f - leftOriginFactor;
	const auto topOriginFactor = m_origins[index->index].y / m_texRects[index->index].height;
	const auto bottomOriginFactor = 1.f - topOriginFactor;

	int i = index->vertIndex;
	m_vertices[i].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 1].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 2].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 3].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), m_rotations[index->index]);
	
	m_positions[index->index] = m_vertices[index->vertIndex].position;
}

sf::Vector2f RenderBatch::GetScale(BatchIndex* index)
{
	return m_scales[index->index];
}

void RenderBatch::SetRect(BatchIndex* index, const sf::IntRect& rect)
{
	const auto originalRect = m_texRects[index->index];
	m_texRects[index->index] = rect;
	UpdateTexCoords(index);

	sf::Vector2f difFactor(rect.width / static_cast<float>(originalRect.width), rect.height / static_cast<float>(originalRect.height));
	sf::Vector2f originPoint = m_positions[index->index] + sf::Vector2f(m_origins[index->index].x * m_scales[index->index].x, m_origins[index->index].y * m_scales[index->index].y);

	for (unsigned i = index->vertIndex; i < index->vertIndex + index->vertCount; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originPoint;
		m_vertices[i].position = originPoint + sf::Vector2f(dif.x * difFactor.x, dif.y * difFactor.y);
	}
}

sf::IntRect RenderBatch::GetRect(BatchIndex* index)
{
	return m_texRects[index->index];
}

void RenderBatch::SetColor(BatchIndex* index, const sf::Color& color)
{
	for (size_t i = index->vertIndex; i < index->vertIndex + index->vertCount; i++)
	{
		m_vertices[i].color = color;
	}
	
}

sf::Color RenderBatch::GetColor(BatchIndex* index)
{
	return m_vertices[index->vertIndex].color;
}

void RenderBatch::UpdateTexCoords(BatchIndex* index)
{
	float left = static_cast<float>(m_texRects[index->index].left);
	float right = left + m_texRects[index->index].width;
	float top = static_cast<float>(m_texRects[index->index].top);
	float bottom = top + m_texRects[index->index].height;

	unsigned i = index->vertIndex;

	m_vertices[i++].texCoords = sf::Vector2f(left, top);
	m_vertices[i++].texCoords = sf::Vector2f(right, top);
	m_vertices[i++].texCoords = sf::Vector2f(right, bottom);
	m_vertices[i].texCoords = sf::Vector2f(left, bottom);
}

sf::Texture* RenderBatch::GetTexture() 
{
	return m_texture.get();
}

void RenderBatch::RemoveDeletedElements()
{
	size_t last = 0;
	for (size_t i = 0; i < m_texRects.size(); ++i, ++last)
	{
		// Set i to the index of the first element that is not to be removed
		while (true)
		{
			bool found = false;
			for (auto& removed : m_removedIndices) 
			{
				if (removed->index == i)
				{
					found = true;
					break;
				}
			}
			if (found) ++i;
			else break;
		}

		if (i >= m_texRects.size()) break;

		// Replace the element being removed with the next element not being removed
		m_vertices[last * 4] = m_vertices[i * 4];
		m_vertices[(last * 4) + 1] = m_vertices[(i * 4) + 1];
		m_vertices[(last * 4) + 2] = m_vertices[(i * 4) + 2];
		m_vertices[(last * 4) + 3] = m_vertices[(i * 4) + 3];

		m_texRects[last] = m_texRects[i];
		m_positions[last] = m_positions[i];
		m_scales[last] = m_scales[i];
		m_origins[last] = m_origins[i];
		m_rotations[last] = m_rotations[i];

		// Swap the pointer contents so users have the correct pointer
		m_indices[last].swap(m_indices[i]);
		m_indices[last]->index = last; // indices have changed
		m_indices[last]->vertIndex = last * 4; // indices have changed
	}

	// Truncate vectors so that removed data is deleted
	m_vertices.resize(last * 4);
	m_texRects.resize(last);
	m_positions.resize(last);
	m_scales.resize(last);
	m_origins.resize(last);
	m_rotations.resize(last);
	m_indices.resize(last);
}

void RenderBatch::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	if (!m_removedIndices.empty())
	{
		RemoveDeletedElements();
		m_removedIndices.clear();
	}

	if (m_vertices.size() == 0)
		return;

	if (m_texture != nullptr)
		states.texture = m_texture.get();

	DRAW_CALLS++;
	target.draw(&m_vertices[0], m_vertices.size(), m_primType, states);
}
