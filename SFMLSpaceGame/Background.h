#pragma once
#include "Component.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class Position;

class Background : public Component
{
private:
	Position* m_position{ nullptr };
	std::shared_ptr<sf::Texture> m_tex;
	sf::Sprite m_sprite;
	int m_resourceID;

public:
	explicit Background(int resourceID) 
		: m_resourceID(resourceID)
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};