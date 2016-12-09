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
	  m_transforms(),
	  m_vertices(),
	  m_transformedVertices(),
	  m_texRects(),
	  m_primType(sf::Quads)
{
}

RenderBatch::RenderBatch(sf::PrimitiveType primType)
	: m_texture(nullptr),
	  m_transforms(),
	  m_vertices(),
	  m_transformedVertices(),
	  m_texRects(),
	  m_primType(primType)
{
}

BatchIndex* RenderBatch::Add(size_t vertCount)
{
	auto index = m_indices.size();
	auto vertIndex = m_vertices.size();

	auto texSize = m_texture != nullptr 
				? m_texture->getSize()
				: sf::Vector2u(0, 0);

	m_texRects.push_back(sf::IntRect(0, 0, static_cast<int>(texSize.x), static_cast<int>(texSize.y)));

	sf::Transformable trans{};
	trans.setPosition(0, 0);
	trans.setScale(1, 1);
	trans.setOrigin(0, 0);
	trans.setRotation(0);
	m_transforms.push_back(trans);

	if (vertCount == 4)
	{
		auto vert1 = sf::Vertex(sf::Vector2f(0, 0), sf::Color::White);
		auto vert2 = sf::Vertex(sf::Vector2f(texSize.x, 0), sf::Color::White);
		auto vert3 = sf::Vertex(sf::Vector2f(texSize.x, texSize.y), sf::Color::White);
		auto vert4 = sf::Vertex(sf::Vector2f(0, texSize.y), sf::Color::White);

		vert1.texCoords = sf::Vector2f(0, 0);
		vert2.texCoords = sf::Vector2f(texSize.x, 0);
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
	}
	else
	{
		auto vert = sf::Vertex({}, sf::Color::White);
		for (size_t i = 0; i < vertCount; i++)
		{
			m_vertices.push_back(vert);
			m_transformedVertices.push_back(vert);
		}
	}
	

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
	size_t numRemoved = m_removedIndices.size();
	size_t numVertsRemoved = 0;
	for (auto& i : m_removedIndices)
	{
		numVertsRemoved += i->vertCount;
	}

	// The index into the indices vector
	// specifying which element to swap
	// an old remove vertex-set to
	int swapPoint = m_indices.size() - 1;
	// For each index to be removed...
	for (size_t i = 0; i < m_removedIndices.size(); i++)
	{
		// Find the next index at the end of the vector
		// that is not being removed
		for (; swapPoint >= 0; swapPoint--)
		{
			bool needsToBeRemoved = false;
			for (auto& ri : m_removedIndices)
			{
				// If the possible swap point is to be removed
				if (ri->index == m_indices[swapPoint]->index)
				{
					needsToBeRemoved = true;
					break;
				}
			}

			// If it doesn't need to be removed, we have found the index to swap to
			if (!needsToBeRemoved)
			{
				break;
			}
		}

		// If the index to remove is already at the end of the vector
		if (swapPoint < 0 
			|| m_indices[swapPoint]->index <= m_removedIndices[i]->index)
		{
			continue;
		}
			
		Replace(m_removedIndices[i]->index, m_indices[swapPoint]->index);
		swapPoint--;
	}

	// Truncate vectors so that removed data is deleted
	m_vertices.resize(m_vertices.size() - numVertsRemoved);
	m_transformedVertices.resize(m_transformedVertices.size() - numVertsRemoved);

	auto newCount = m_indices.size() - numRemoved;
	m_texRects.resize(newCount);
	m_indices.resize(newCount);
	m_transforms.resize(newCount);
}

void RenderBatch::Replace(size_t a, size_t b)
{
	// If we can simply swap the replaced with the saved
	if (m_indices[a]->vertCount == m_indices[b]->vertCount)
	{
		Replace(a, b, m_indices[b]->vertCount);
	}
	// If the vert-set to be removed is smaller than what we are placing there
	// TODO: Search for a vert-set that can fit before choosing to insert any verts
	else if (m_indices[a]->vertCount < m_indices[b]->vertCount)
	{
		// how many verts do we need to insert
		auto extraVerts = m_indices[b]->vertCount - m_indices[a]->vertCount;

		// insert verts to make room 
		m_vertices.insert(m_vertices.begin() + m_indices[a]->vertIndex, extraVerts, sf::Vertex());

		// Adjust vertices based on what was inserted
		for (size_t j = a + 1; j < m_indices.size(); j++)
		{
			m_indices[j]->vertIndex += extraVerts;
		}

		Replace(a, b, m_indices[b]->vertCount);
	}
	// vert-set to be removed is larger than what is being placed there
	else
	{
		// how many verts do we need to remove
		auto extraVerts = m_indices[a]->vertCount - m_indices[b]->vertCount;

		// Iterators for the section to be removed
		auto startIt = m_vertices.begin() + m_indices[a]->vertIndex + (m_indices[a]->vertCount - extraVerts);
		auto endIt = m_vertices.begin() + m_indices[a]->vertIndex + m_indices[a]->vertCount;

		// Remove the verts
		m_vertices.erase(startIt, endIt);

		// Adjust vertices based on what was removed
		for (size_t j = a + 1; j < m_indices.size(); j++)
		{
			m_indices[j]->vertIndex -= extraVerts;
		}

		Replace(a, b, m_indices[b]->vertCount);
	}
}

void RenderBatch::Replace(size_t a, size_t b, size_t count) 
{
	auto vertIndexA = m_indices[a]->vertIndex;
	auto vertIndexB = m_indices[b]->vertIndex;

	// Copy B's verts into A's
	for (size_t i = 0; i < count; i++) 
	{
		m_vertices[vertIndexA + i] = m_vertices[vertIndexB + i];
	}

	m_texRects[a] = m_texRects[b];
	m_transforms[a] = m_transforms[b];

	// Swap the pointer contents so users have the correct pointer
	m_indices[a].swap(m_indices[b]);
	m_indices[a]->index = a; // indices have changed
	m_indices[a]->vertIndex = vertIndexA; // indices have changed
	m_indices[a]->vertCount = count;
	
	m_indices[b]->index = b;
	m_indices[b]->vertIndex = vertIndexB;
	m_indices[b]->vertCount = count;
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
