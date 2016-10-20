// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <RenderBatch.h>
#include <PolarVector.h>
#include <SFML\Graphics\RenderTarget.hpp>
#include <ResourceLoader.h>

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

const float TO_RAD = M_PI / 180.f;

std::unordered_map<std::string, std::unique_ptr<RenderBatch>> RenderBatch::m_batches;

RenderBatch* RenderBatch::Get(const std::string& texName)
{
	auto it = m_batches.find(texName);
	if (it != m_batches.end())
		return it->second.get();

	auto batch = std::make_unique<RenderBatch>(LoadTexture(texName));
	
	m_batches.insert(make_pair(texName, std::move(batch)));

	return m_batches.find(texName)->second.get();
}

void RenderBatch::RenderAll(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto& b : m_batches) 
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

	m_texRects.push_back(sf::IntRect(0, 0, (int)texSize.x, (int)texSize.y));
	m_positions.push_back(sf::Vector2f(0, 0));
	m_scales.push_back(sf::Vector2f(0, 0));
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
	Move(index, (pos - m_positions[index]));
}

sf::Vector2f RenderBatch::GetPosition(const unsigned index)
{
	return m_positions[index] + m_origins[index];
}

void RenderBatch::SetOrigin(const unsigned index, const sf::Vector2f& origin)
{
	sf::Vector2f scaledOrigin = sf::Vector2f(origin.x * m_scales[index].x, origin.y * m_scales[index].y);

	auto originalOrigin = m_origins[index];

	Move(index, -(scaledOrigin - originalOrigin));

	m_origins[index] = scaledOrigin;
}

sf::Vector2f RenderBatch::GetOrigin(const unsigned index)
{
	return m_origins[index];
}

void RenderBatch::SetRotation(const unsigned index, float ang)
{
	/*float ang_t = ang;
	m_angles[index] = fmod(ang, 360.f);
	PolarVector P = TurnToPolar(m_positions[index]); P.t = ang_t;
	SetPosition(index, P.TurnToRectangular());*/
	auto originalAngle = m_angles[index];
	m_angles[index] = fmod(ang, 360.f);

	float ang_t = (m_angles[index] - originalAngle) * TO_RAD;
	sf::Vector2f tmp = m_positions[index];
	sf::Vector2f off = -m_origins[index];
	//sf::Vector2f off(-m_texRects[index].width / 2.f, -m_texRects[index].height / 2.f);
	//off = sf::Vector2f(off.x * m_scales[index].x, off.y * m_scales[index].y);

	SetPosition(index, off);
	unsigned limit = (index * 4) + 4;
	for (unsigned i = (index * 4); i < limit; i++) 
	{
		PolarVector p(TurnToPolar(m_vertices[i].position));
		p.t += ang_t;
		m_vertices[i].position = p.TurnToRectangular();
	}
	Move(index, tmp - off);
}

float RenderBatch::GetRotation(const unsigned index)
{
	return m_angles[index];
}

void RenderBatch::SetScale(const unsigned index, const sf::Vector2f& scale)
{
	m_scales[index].x = scale.x;
	m_scales[index].y = scale.y;
	for (unsigned i = index * 4; i < (index * 4) + 4; ++i)
	{
		m_vertices[i].position.x *= scale.x;
		m_vertices[i].position.y *= scale.y;
	}
}

sf::Vector2f RenderBatch::GetScale(const unsigned index)
{
	return m_scales[index];
}

void RenderBatch::SetTextureRect(const unsigned int index, const sf::IntRect& rect)
{
	m_texRects[index] = rect;
	UpdateTexCoords(index);
	UpdateVertexCoords(index, false);
}

sf::IntRect RenderBatch::GetTextureRect(const unsigned index) 
{
	return m_texRects[index];
}

void RenderBatch::UpdateTexCoords(const unsigned index) 
{
	float left = static_cast<float>(m_texRects[index].left);
	float right = left + m_texRects[index].width;
	float top = static_cast<float>(m_texRects[index].top);
	float bottom = top + m_texRects[index].height;

	unsigned I = index * 4;

	m_vertices[I++].texCoords = sf::Vector2f(left, top);
	m_vertices[I++].texCoords = sf::Vector2f(left, bottom);
	m_vertices[I++].texCoords = sf::Vector2f(right, bottom);
	m_vertices[I].texCoords = sf::Vector2f(right, top);
}

void RenderBatch::UpdateVertexCoords(const unsigned index, bool reset) 
{
	sf::Vector2u S;
	S.x = m_texRects[index].width * m_scales[index].x;
	S.y = m_texRects[index].height * m_scales[index].y;

	unsigned I = index * 4;

	m_vertices[I++].position = sf::Vector2f(0, 0);
	m_vertices[I++].position = sf::Vector2f(0, S.y);
	m_vertices[I++].position = sf::Vector2f(S.x, S.y);
	m_vertices[I].position = sf::Vector2f(S.x, 0);

	if (reset)
	{
		m_positions[index].x = 0.f;
		m_positions[index].y = 0.f;
	}
	else
	{
		Move(index, m_positions[index]);
	}
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
