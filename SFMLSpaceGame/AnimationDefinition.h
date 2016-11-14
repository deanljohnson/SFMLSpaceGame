#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class AnimationDefinition
{
private:
	std::shared_ptr<sf::Texture> m_texture;
	std::vector<sf::IntRect> m_frames;
	// stores an additional rect per animation
	// generally used for colliders
	std::vector<sf::IntRect> m_contentRects; 
	sf::Vector2f m_frameSize;
	float m_length;

public:
	AnimationDefinition(std::shared_ptr<sf::Texture> texture, const sf::Vector2f& frameSize, float length, int numFrames = 0);

	inline std::shared_ptr<sf::Texture> GetTexture() const { return m_texture; }
	inline sf::Vector2f GetFrameSize() const { return m_frameSize; }
	inline float GetLength() const { return m_length; }

	inline int FrameCount() const { return m_frames.size(); }

	inline sf::IntRect GetFrame(const int index) const
	{
		return m_frames[index];
	}

	inline sf::IntRect GetContentRect(const int index) const
	{
		return m_contentRects[index];
	}

	inline void SetContentRect(const int index, const sf::IntRect& rect)
	{
		m_contentRects[index] = rect;
	}

	inline sf::IntRect operator[](const int index) const
	{
		return m_frames[index];
	}
};