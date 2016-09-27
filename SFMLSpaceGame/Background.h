#pragma once
#include "Component.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

class Position;

class Background : public Component
{
private:
	Position* m_position{ nullptr };
	sf::Texture m_tex;
	sf::Sprite m_sprite;
	std::wstring m_path;
public:
	explicit Background(std::wstring path) 
		: m_path(path)
	{}

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};