#include <Animation.h>

Animation::Animation(ResourceID atlasID, sf::Vector2f frameSize)
{
	m_atlas = LoadBuiltInTexture(atlasID);

	for (int j = 0; j < m_atlas->getSize().y / frameSize.y; j++)
	{
		for (int i = 0; i < m_atlas->getSize().x / frameSize.x; i++)
		{
			m_frames.push_back(sf::IntRect(frameSize.x * i, frameSize.y * j, frameSize.x, frameSize.y));
		}
	}

	m_frames.shrink_to_fit();

	m_counter = 0;
	m_length = 1.f;
	m_frameLength = 1.f / m_frames.size();
	m_speed = 1.f;
	m_currentFrame = 0;
}

void Animation::Update(float deltaTime)
{
	m_counter += (deltaTime * m_speed);

	while (m_counter > m_frameLength)
	{
		m_counter -= m_frameLength;

		m_currentFrame++;
		m_currentFrame %= m_frames.size();
	}
}

void Animation::SetLength(float length)
{
	m_length = length;
	m_frameLength = m_length / m_frames.size();
}

void Animation::SetSpeed(float speed)
{
	m_speed = speed;
}

sf::IntRect Animation::GetCurrentFrame() const
{
	return m_frames[m_currentFrame];
}

sf::Texture* Animation::GetTexture() const
{
	return m_atlas.get();
}