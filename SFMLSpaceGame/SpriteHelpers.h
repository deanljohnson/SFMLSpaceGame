#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

enum class OriginOption;

namespace sf{
	class Sprite;
}

namespace SpriteHelpers
{
	void SetOrigin(sf::Sprite* s, OriginOption option);
	sf::Vector2f GetOrigin(const sf::IntRect& rect, OriginOption option);
}