#pragma once
#include "Component.h"
#include <ResourceLoader.h>
#include <SFML/Graphics/Sprite.hpp>

class Position;

class TilingBackground : public Component
{
private:
	Position* m_position{ nullptr };
	std::shared_ptr<sf::Texture> m_tex;
	sf::Sprite m_sprite;
	ResourceID m_resourceID;

public:
	explicit TilingBackground(ResourceID resourceID)
		: m_resourceID(resourceID)
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};