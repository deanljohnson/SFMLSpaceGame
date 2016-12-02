// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <AnimationDefinition.h>

AnimationDefinition::AnimationDefinition(std::shared_ptr<sf::Texture> texture, const sf::Vector2f& frameSize, float length, int numFrames)
	: m_texture(texture),
	  m_frameSize(frameSize),
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