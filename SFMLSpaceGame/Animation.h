#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include "ResourceLoader.h"

class Animation
{
private:
	std::shared_ptr<sf::Texture> m_atlas;
	std::vector<sf::IntRect> m_frames;

	float m_counter;
	float m_length;
	float m_frameLength;
	float m_speed;
	int m_currentFrame;

public:
	// Creates an animation with the given texture ID.
	// Divides the texture into frames of the given size
	// moving left to right, top to bottom
	explicit Animation(ResourceID atlasID, sf::Vector2f frameSize);

	void Update(float deltaTime);

	void SetLength(float length);
	void SetSpeed(float speed);

	sf::IntRect GetCurrentFrame() const;
	sf::Texture* GetTexture() const;
};