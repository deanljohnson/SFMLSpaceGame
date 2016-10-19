// based on https://github.com/SFML/SFML/wiki/Source:-High-Performance-Sprite-Container
#include <RenderBatch.h>
#include <PolarVector.h>
#include <SFML\Graphics\RenderTarget.hpp>

RenderBatch::RenderBatch(std::shared_ptr<sf::Texture> tex) 
{
	m_texture = tex;
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
	Move(index, pos - m_positions[index]);
}

sf::Vector2f RenderBatch::GetPosition(const unsigned index)
{
	return m_positions[index];
}

void RenderBatch::SetRotation(const unsigned index, float ang)
{
	float ang_t = ang;
	m_angles[index] = fmod(ang, 360.f);
	PolarVector P = TurnToPolar(m_positions[index]); P.t = ang_t;
	SetPosition(index, P.TurnToRectangular());
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

void RenderBatch::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	states.texture = m_texture.get();
	target.draw(&m_vertices[0], static_cast<unsigned>(m_vertices.size()), sf::Quads, states);
}
