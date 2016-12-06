// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <AnimationDefinition.h>

AnimationDefinition::AnimationDefinition()
	: m_texId(),
	m_texture(),
	m_frames(),
	m_contentRects(),
	m_length(0.f)
{
}

AnimationDefinition::AnimationDefinition(const std::string& id, const sf::Vector2f& frameSize, float length, int numFrames)
	: m_texId(id),
	m_texture(LoadTexture(id)),
	m_frames(),
	m_contentRects(),
	m_length(length)
{
	for (int j = 0; j < m_texture->getSize().y / frameSize.y; j++)
	{
		for (int i = 0; i < m_texture->getSize().x / frameSize.x; i++)
		{
			if (numFrames != 0 && m_frames.size() == numFrames)
				break;

			m_frames.push_back(sf::IntRect(frameSize.x * i, frameSize.y * j, frameSize.x, frameSize.y));
			m_contentRects.push_back(sf::IntRect(frameSize.x * i, frameSize.y * j, frameSize.x, frameSize.y));
		}
	}

	m_frames.shrink_to_fit();
	m_contentRects.shrink_to_fit();
}

std::shared_ptr<sf::Texture> AnimationDefinition::GetTexture() const
{
	return m_texture;
}

float AnimationDefinition::GetLength() const
{
	return m_length;
}

int AnimationDefinition::FrameCount() const
{
	return m_frames.size();
}

sf::IntRect AnimationDefinition::GetFrame(size_t index) const
{
	return m_frames[index];
}

sf::IntRect AnimationDefinition::GetContentRect(size_t index) const
{
	return m_contentRects[index];
}

void AnimationDefinition::SetContentRect(size_t index, const sf::IntRect& rect)
{
	m_contentRects[index] = rect;
}

std::string AnimationDefinition::GetTypeName()
{
	static std::string name = "anim";
	return name;
}