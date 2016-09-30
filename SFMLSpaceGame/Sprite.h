#pragma once
#include "Position.h"
#include "Rotation.h"
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceLoader.h"

class Sprite : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	sf::Sprite m_sprite;
	ResourceID m_resourceID;
	std::shared_ptr<sf::Texture> m_tex;

public:
	explicit Sprite(ResourceID id) 
		: m_resourceID(id)
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;

	virtual sf::IntRect GetPixelLocalBounds() const;
};