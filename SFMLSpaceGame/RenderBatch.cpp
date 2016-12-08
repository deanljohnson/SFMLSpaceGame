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
	  m_transformedVertices(),
	  m_texRects(),
	  m_transforms(),
	  m_primType(sf::Quads)
{
}

RenderBatch::RenderBatch(sf::PrimitiveType primType)
	: m_texture(nullptr),
	  m_vertices(),
	  m_transformedVertices(),
	  m_texRects(),
	  m_transforms(),
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

	sf::Transformable trans{};
	trans.setPosition(0, 0);
	trans.setScale(1, 1);
	trans.setOrigin(0, 0);
	trans.setRotation(0);
	m_transforms.push_back(trans);

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

	m_transformedVertices.push_back(vert1);
	m_transformedVertices.push_back(vert2);
	m_transformedVertices.push_back(vert3);
	m_transformedVertices.push_back(vert4);

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
	m_transforms[index->index].move(x, y);
}

void RenderBatch::Move(BatchIndex* index, const sf::Vector2f& amt)
{
	m_transforms[index->index].move(amt.x, amt.y);
}

void RenderBatch::SetPosition(BatchIndex* index, const sf::Vector2f& pos)
{
	m_transforms[index->index].setPosition(pos);
}

sf::Vector2f RenderBatch::GetPosition(BatchIndex* index)
{
	return m_transforms[index->index].getPosition();
}

void RenderBatch::SetOrigin(BatchIndex* index, const sf::Vector2f& origin)
{
	m_transforms[index->index].setOrigin(origin);
}

sf::Vector2f RenderBatch::GetOrigin(BatchIndex* index)
{
	return m_transforms[index->index].getOrigin();
}

sf::Vector2f RenderBatch::GetScaledOrigin(BatchIndex* index)
{
	return MultComponents(m_transforms[index->index].getOrigin(), m_transforms[index->index].getScale());
}

void RenderBatch::SetRotation(BatchIndex* index, float ang)
{
	m_transforms[index->index].setRotation(RAD_TO_DEG(ang));
}

float RenderBatch::GetRotation(BatchIndex* index)
{
	return DEG_TO_RAD(m_transforms[index->index].getRotation());
}

void RenderBatch::SetScale(BatchIndex* index, const sf::Vector2f& scale)
{
	m_transforms[index->index].setScale(scale);
}

sf::Vector2f RenderBatch::GetScale(BatchIndex* index)
{
	return m_transforms[index->index].getScale();
}

void RenderBatch::SetRect(BatchIndex* index, const sf::IntRect& rect)
{
	m_texRects[index->index] = rect;
	UpdateTexCoords(index);
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

sf::Vertex& RenderBatch::GetVertex(BatchIndex* index, size_t vert) 
{
	assert(vert < index->vertCount);
	return m_vertices[index->vertIndex + vert];
}

void RenderBatch::RemoveDeletedElements()
{
	size_t last = 0;
	for (size_t i = 0; i < m_indices.size(); ++i, ++last)
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

		if (i >= m_indices.size()) break;

		auto toRemove = last;

		// If we can simply swap the replaced with the saved
		if (m_indices[toRemove]->vertCount == m_indices[i]->vertCount)
		{
			Replace(toRemove, i, m_indices[i]->vertCount);
		}
		// If the vert-set to be removed is smaller than what we are placing there
		// TODO: Search for a vert-set that can fit before choosing to insert any verts
		else if (m_indices[toRemove]->vertCount < m_indices[i]->vertCount)
		{
			// how many verts do we need to insert
			auto extraVerts = m_indices[i]->vertCount - m_indices[toRemove]->vertCount;

			// insert verts to make room 
			for (size_t i = 0; i < extraVerts; i++) 
			{
				m_vertices.insert(m_vertices.begin() + m_indices[toRemove]->vertIndex, sf::Vertex());
			}

			// TODO: adjust all vert indices based on what was inserted and where

			Replace(toRemove, i, m_indices[i]->vertCount);
		}
	}

	// Truncate vectors so that removed data is deleted
	m_vertices.resize(last * 4);
	m_transformedVertices.resize(last * 4);
	m_texRects.resize(last);
	m_indices.resize(last);
	m_transforms.resize(last);
}

void RenderBatch::Replace(size_t a, size_t b, size_t count) 
{
	auto baseVertIndex = a * 4;

	// Replace the element being removed with the next element not being removed
	for (size_t i = 0; i < count; i++) 
	{
		m_vertices[baseVertIndex + i] = m_vertices[(b * 4) + i];
	}

	m_texRects[a] = m_texRects[b];
	m_transforms[a] = m_transforms[b];

	// Swap the pointer contents so users have the correct pointer
	m_indices[a].swap(m_indices[b]);
	m_indices[a]->index = a; // indices have changed
	m_indices[a]->vertIndex = baseVertIndex; // indices have changed
	m_indices[a]->vertCount = count;
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

	// Transform all vertices to be rendered
	for (size_t i = 0; i < m_transforms.size(); i++) 
	{
		const auto& trans = m_transforms[i].getTransform();

		for (size_t j = m_indices[i]->vertIndex; j < m_indices[i]->vertIndex + m_indices[i]->vertCount; j++)
		{
			m_transformedVertices[j].color = m_vertices[j].color;
			m_transformedVertices[j].position = trans.transformPoint(m_vertices[j].position);
			m_transformedVertices[j].texCoords = m_vertices[j].texCoords;
		}
	}

	DRAW_CALLS++;
	target.draw(&m_transformedVertices[0], m_vertices.size(), m_primType, states);
}
