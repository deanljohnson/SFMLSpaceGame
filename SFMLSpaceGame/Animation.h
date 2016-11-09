#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <AnimationDefinition.h>

class Animation
{
private:
	std::shared_ptr<AnimationDefinition> m_definition;

	std::shared_ptr<sf::Texture> m_atlas;

	float m_counter;
	float m_frameLength;
	float m_speed;
	int m_currentFrame;
public:
	explicit Animation(std::shared_ptr<AnimationDefinition> definition);

	void Update(float deltaTime);

	float GetLength();

	void SetSpeed(float speed);
	float GetSpeed();

	sf::IntRect GetCurrentFrame() const;
	sf::Texture* GetTexture() const;
};