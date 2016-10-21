// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <RenderBatch.h>
#include <VectorMath.h>
#include <SFML\Graphics\RenderTarget.hpp>

std::unordered_map<std::string, std::unique_ptr<RenderBatch>> RenderBatch::m_stringBatches;
std::unordered_map<ResourceID, std::unique_ptr<RenderBatch>> RenderBatch::m_resourceBatches;

RenderBatch* RenderBatch::Get(const std::string& texName)
{
	auto it = m_stringBatches.find(texName);
	if (it != m_stringBatches.end())
		return it->second.get();

	auto batch = std::make_unique<RenderBatch>(LoadTexture(texName));
	
	auto inserted = m_stringBatches.insert(make_pair(texName, move(batch)));

	return inserted.first->second.get();
}

RenderBatch* RenderBatch::Get(ResourceID texID)
{
	auto it = m_resourceBatches.find(texID);
	if (it != m_resourceBatches.end())
		return it->second.get();

	auto batch = std::make_unique<RenderBatch>(LoadTexture(texID));

	auto inserted = m_resourceBatches.insert(make_pair(texID, move(batch)));

	return inserted.first->second.get();
}

void RenderBatch::RenderAll(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& b : m_stringBatches) 
	{
		b.second->Render(target, states);
	}
	for (auto& b : m_resourceBatches)
	{
		b.second->Render(target, states);
	}
}

RenderBatch::RenderBatch(std::shared_ptr<sf::Texture> tex) 
	: m_vertices(),
	  m_texRects(),
	  m_positions(),
	  m_scales(),
	  m_origins(),
	  m_rotations()
{
	m_texture = tex;
}

BatchIndex* RenderBatch::Add()
{
	auto index = m_vertices.size() / 4;
	auto texSize = m_texture->getSize();

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

	BatchIndex* i = new BatchIndex(index);
	m_indices.push_back(std::unique_ptr<BatchIndex>(i));

	return i;
}

void RenderBatch::Remove(BatchIndex* index)
{
	m_removedIndices.push_back(index);

	/*m_vertices.erase(m_vertices.begin() + (index * 4), m_vertices.begin() + (index * 4) + 4);
	m_texRects.erase(m_texRects.begin() + index);
	m_positions.erase(m_positions.begin() + index);
	m_scales.erase(m_scales.begin() + index);
	m_origins.erase(m_origins.begin() + index);
	m_rotations.erase(m_rotations.begin() + index);*/
}

void RenderBatch::Move(BatchIndex* index, float x, float y)
{
	unsigned int limit = (index->index * 4) + 4;
	for (unsigned int i = index->index * 4; i < limit; i++) 
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

	for (unsigned i = index->index * 4; i < (index->index * 4) + 4; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originScaled;
		m_vertices[i].position = sf::Vector2f((cosT * dif.x) - (sinT * dif.y) + originScaled.x,
											(sinT * dif.x) + (cosT * dif.y) + originScaled.y);
	}

	m_positions[index->index] = m_vertices[index->index * 4].position;
}

float RenderBatch::GetRotation(BatchIndex* index)
{
	return m_rotations[index->index].GetAngle();
}

void RenderBatch::SetScale(BatchIndex* index, const sf::Vector2f& scale)
{
 	sf::Vector2f originalScale = m_scales[index->index];
	m_scales[index->index] = scale;
	
	sf::Vector2f originalSize(m_texRects[index->index].width * originalScale.x, m_texRects[index->index].height * originalScale.y);
	sf::Vector2f targetSize(m_texRects[index->index].width * scale.x, m_texRects[index->index].height * scale.y);
	sf::Vector2f sizeDif = targetSize - originalSize;

	float leftOriginFactor = m_origins[index->index].x / m_texRects[index->index].width;
	float rightOriginFactor = 1.f - leftOriginFactor;
	float topOriginFactor = m_origins[index->index].y / m_texRects[index->index].height;
	float bottomOriginFactor = 1.f - topOriginFactor;

	int i = index->index * 4;
	m_vertices[i].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 1].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 2].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), m_rotations[index->index]);
	m_vertices[i + 3].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), m_rotations[index->index]);
	
	m_positions[index->index] = m_vertices[i].position;
}

sf::Vector2f RenderBatch::GetScale(BatchIndex* index)
{
	return m_scales[index->index];
}

void RenderBatch::SetTextureRect(BatchIndex* index, const sf::IntRect& rect)
{
	sf::IntRect originalRect = m_texRects[index->index];
	m_texRects[index->index] = rect;
	UpdateTexCoords(index);

	sf::Vector2f difFactor(rect.width / static_cast<float>(originalRect.width), rect.height / static_cast<float>(originalRect.height));
	sf::Vector2f originPoint = m_positions[index->index] + sf::Vector2f(m_origins[index->index].x * m_scales[index->index].x, m_origins[index->index].y * m_scales[index->index].y);

	for (unsigned i = index->index * 4; i < (index->index * 4) + 4; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originPoint;
		m_vertices[i].position = originPoint + sf::Vector2f(dif.x * difFactor.x, dif.y * difFactor.y);
	}
}

sf::IntRect RenderBatch::GetTextureRect(BatchIndex* index)
{
	return m_texRects[index->index];
}

void RenderBatch::SetColor(BatchIndex* index, const sf::Color& color)
{
	unsigned i = index->index * 4;

	m_vertices[i++].color = color;
	m_vertices[i++].color = color;
	m_vertices[i++].color = color;
	m_vertices[i].color = color;
}

sf::Color RenderBatch::GetColor(BatchIndex* index)
{
	return m_vertices[index->index * 4].color;
}

void RenderBatch::UpdateTexCoords(BatchIndex* index)
{
	float left = static_cast<float>(m_texRects[index->index].left);
	float right = left + m_texRects[index->index].width;
	float top = static_cast<float>(m_texRects[index->index].top);
	float bottom = top + m_texRects[index->index].height;

	unsigned i = index->index * 4;

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
	int last = 0;
	for (int i = 0; i < m_texRects.size(); ++i, ++last)
	{
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

		m_vertices[last * 4] = m_vertices[(last + 1) * 4];
		m_vertices[(last * 4) + 1] = m_vertices[((last + 1) * 4) + 1];
		m_vertices[(last * 4) + 2] = m_vertices[((last + 1) * 4) + 2];
		m_vertices[(last * 4) + 3] = m_vertices[((last + 1) * 4) + 3];

		m_texRects[last] = m_texRects[i];
		m_positions[last] = m_positions[i];
		m_scales[last] = m_scales[i];
		m_origins[last] = m_origins[i];
		m_rotations[last] = m_rotations[i];
	}

	m_vertices.resize(last * 4);
	m_texRects.resize(last);
	m_positions.resize(last);
	m_scales.resize(last);
	m_origins.resize(last);
	m_rotations.resize(last);

	for (int i = 0; i < m_removedIndices.size(); i++) 
	{
		for (int j = 0; j < m_indices.size(); j++)
		{
			if (m_removedIndices[i]->index == m_indices[j]->index) 
			{
				m_indices.erase(m_indices.begin() + j);
				j--;
			}
			else if (m_removedIndices[i]->index < m_indices[j]->index)
			{
				m_indices[j]->index--;
			}
				
		}
	}
}

void RenderBatch::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	if (!m_removedIndices.empty())
	{
		RemoveDeletedElements();
		m_removedIndices.clear();
	}

	states.texture = m_texture.get();
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);
}
