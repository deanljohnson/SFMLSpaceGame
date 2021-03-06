// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Animation.h>

Animation::Animation(std::shared_ptr<AnimationDefinition> definition)
	: m_definition(definition)
{
	m_counter = 0;
	m_frameLength = m_definition->GetLength() / m_definition->FrameCount();
	m_speed = 1.f;
	m_currentFrame = 0;
}

bool Animation::Update(float deltaTime)
{
	bool changed = false;
	m_counter += (deltaTime * m_speed);

	while (m_counter > m_frameLength)
	{
		m_counter -= m_frameLength;

		m_currentFrame++;
		m_currentFrame %= m_definition->FrameCount();

		changed = true;
	}

	return changed;
}
float Animation::GetLength()
{
	return m_definition->GetLength();
}

void Animation::SetSpeed(float speed)
{
	m_speed = speed;
}

float Animation::GetSpeed()
{
	return m_speed;
}

sf::IntRect Animation::GetCurrentFrame() const
{
	return m_definition->GetFrame(m_currentFrame);
}

sf::IntRect Animation::operator[](size_t index) const
{
	return m_definition->GetFrame(index);
}

sf::Texture* Animation::GetTexture() const
{
	return m_definition->GetTexture().get();
}