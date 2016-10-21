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
	  m_angles()
{
	m_texture = tex;
}

unsigned RenderBatch::Add() 
{
	auto index = m_vertices.size() / 4;
	auto texSize = m_texture->getSize();

	m_texRects.push_back(sf::IntRect(0, 0, static_cast<int>(texSize.x), static_cast<int>(texSize.y)));
	m_positions.push_back(sf::Vector2f(0, 0));
	m_scales.push_back(sf::Vector2f(1, 1));
	m_origins.push_back(sf::Vector2f(0, 0));
	m_angles.push_back(0.f);

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

	return index;
}

void RenderBatch::Move(const unsigned index, float x, float y)
{
	unsigned int limit = (index * 4) + 4;
	for (unsigned int i = index * 4; i < limit; i++) 
	{
		m_vertices[i].position.x += x;
		m_vertices[i].position.y += y;
	}

	m_positions[index].x += x;
	m_positions[index].y += y;
}

void RenderBatch::Move(const unsigned index, const sf::Vector2f& amt)
{
	Move(index, amt.x, amt.y);
}

void RenderBatch::SetPosition(const unsigned index, const sf::Vector2f& pos)
{
	Move(index, pos - (m_positions[index] + Rotate(MultComponents(m_origins[index], m_scales[index]),
													m_angles[index])));
}

sf::Vector2f RenderBatch::GetPosition(const unsigned index)
{
	sf::Vector2f origin = Rotate(MultComponents(m_origins[index], m_scales[index]), 
								 m_angles[index]);

	return m_positions[index] + origin;
}

void RenderBatch::SetOrigin(const unsigned index, const sf::Vector2f& origin)
{
	sf::Vector2f originDif = origin - m_origins[index];
	originDif = Rotate(MultComponents(originDif, m_scales[index]), m_angles[index]);
	Move(index, -originDif);
	m_origins[index] = origin;
}

sf::Vector2f RenderBatch::GetOrigin(const unsigned index)
{
	return m_origins[index];
}

sf::Vector2f RenderBatch::GetScaledOrigin(const unsigned index)
{
	return MultComponents(m_origins[index], m_scales[index]);
}

void RenderBatch::SetRotation(const unsigned index, float ang)
{
	auto originalAngle = m_angles[index];
	m_angles[index] = ang;

	// Determine the amount to rotate by
	float ang_t = m_angles[index] - originalAngle;

	sf::Vector2f originScaled = GetScaledOrigin(index);
	originScaled = Rotate(originScaled, originalAngle) + m_positions[index];

	// at this point, originScaled is the origin of our quad
	float cosT = cosf(ang_t);
	float sinT = sinf(ang_t);

	for (unsigned i = index * 4; i < (index * 4) + 4; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originScaled;
		m_vertices[i].position = sf::Vector2f((cosT * dif.x) - (sinT * dif.y) + originScaled.x,
											(sinT * dif.x) + (cosT * dif.y) + originScaled.y);
	}

	m_positions[index] = m_vertices[index * 4].position;
}

float RenderBatch::GetRotation(const unsigned index)
{
	return m_angles[index];
}

void RenderBatch::SetScale(const unsigned index, const sf::Vector2f& scale)
{
 	sf::Vector2f originalScale = m_scales[index];
	m_scales[index] = scale;
	
	sf::Vector2f originalSize(m_texRects[index].width * originalScale.x, m_texRects[index].height * originalScale.y);
	sf::Vector2f targetSize(m_texRects[index].width * scale.x, m_texRects[index].height * scale.y);
	sf::Vector2f sizeDif = targetSize - originalSize;

	float leftOriginFactor = m_origins[index].x / m_texRects[index].width;
	float rightOriginFactor = 1.f - leftOriginFactor;
	float topOriginFactor = m_origins[index].y / m_texRects[index].height;
	float bottomOriginFactor = 1.f - topOriginFactor;

	b2Rot rot(m_angles[index]);

	int i = index * 4;
	m_vertices[i].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), rot);
	m_vertices[i + 1].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, -topOriginFactor * sizeDif.y), rot);
	m_vertices[i + 2].position += Rotate(sf::Vector2f(rightOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), rot);
	m_vertices[i + 3].position += Rotate(sf::Vector2f(-leftOriginFactor * sizeDif.x, bottomOriginFactor * sizeDif.y), rot);
	
	m_positions[index] = m_vertices[i].position;
}

sf::Vector2f RenderBatch::GetScale(const unsigned index)
{
	return m_scales[index];
}

void RenderBatch::SetTextureRect(const unsigned index, const sf::IntRect& rect)
{
	sf::IntRect originalRect = m_texRects[index];
	m_texRects[index] = rect;
	UpdateTexCoords(index);

	sf::Vector2f difFactor(rect.width / static_cast<float>(originalRect.width), rect.height / static_cast<float>(originalRect.height));
	sf::Vector2f originPoint = m_positions[index] + sf::Vector2f(m_origins[index].x * m_scales[index].x, m_origins[index].y * m_scales[index].y);

	for (unsigned i = index * 4; i < (index * 4) + 4; i++)
	{
		sf::Vector2f dif = m_vertices[i].position - originPoint;
		m_vertices[i].position = originPoint + sf::Vector2f(dif.x * difFactor.x, dif.y * difFactor.y);
	}
}

sf::IntRect RenderBatch::GetTextureRect(const unsigned index) 
{
	return m_texRects[index];
}

void RenderBatch::SetColor(const unsigned index, const sf::Color& color)
{
	unsigned i = index * 4;

	m_vertices[i++].color = color;
	m_vertices[i++].color = color;
	m_vertices[i++].color = color;
	m_vertices[i].color = color;
}

sf::Color RenderBatch::GetColor(const unsigned index)
{
	return m_vertices[index * 4].color;
}

void RenderBatch::UpdateTexCoords(const unsigned index) 
{
	float left = static_cast<float>(m_texRects[index].left);
	float right = left + m_texRects[index].width;
	float top = static_cast<float>(m_texRects[index].top);
	float bottom = top + m_texRects[index].height;

	unsigned i = index * 4;

	m_vertices[i++].texCoords = sf::Vector2f(left, top);
	m_vertices[i++].texCoords = sf::Vector2f(right, top);
	m_vertices[i++].texCoords = sf::Vector2f(right, bottom);
	m_vertices[i].texCoords = sf::Vector2f(left, bottom);
}

sf::Texture* RenderBatch::GetTexture() 
{
	return m_texture.get();
}

void RenderBatch::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	states.texture = m_texture.get();
	target.draw(&m_vertices[0], m_vertices.size(), sf::Quads, states);
}
