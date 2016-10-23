#include <AnimationDefinition.h>

AnimationDefinition::AnimationDefinition(std::shared_ptr<sf::Texture> texture, const sf::Vector2f& frameSize, float length)
	: m_texture(texture),
	  m_frameSize(frameSize),
	  m_length(length)
{
	for (int j = 0; j < m_texture->getSize().y / frameSize.y; j++)
	{
		for (int i = 0; i < m_texture->getSize().x / frameSize.x; i++)
		{
			m_frames.push_back(sf::IntRect(frameSize.x * i, frameSize.y * j, frameSize.x, frameSize.y));
		}
	}

	m_frames.shrink_to_fit();
}