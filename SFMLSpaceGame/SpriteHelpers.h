#pragma once

enum class OriginOption;

namespace sf{
	class Sprite;
}

namespace SpriteHelpers
{
	void SetOrigin(sf::Sprite* s, OriginOption option);
}