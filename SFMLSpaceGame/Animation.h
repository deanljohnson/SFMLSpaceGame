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

	void InitializeFrames(const sf::Vector2f& frameSize);
public:
	// Creates an animation with the given texture ID.
	// Divides the texture into frames of the given size
	// moving left to right, top to bottom
	explicit Animation(ResourceID atlasID, const sf::Vector2f& frameSize);
	explicit Animation(const std::string& atlasID, const sf::Vector2f& frameSize);

	void Update(float deltaTime);

	void SetLength(float length);
	float GetLength();

	void SetSpeed(float speed);
	float GetSpeed();

	sf::IntRect GetCurrentFrame() const;
	sf::Texture* GetTexture() const;
};