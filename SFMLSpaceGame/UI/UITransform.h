#pragma once
#include <SFML/System/Vector2.hpp>
#include <utility>

struct UITransform
{
	UITransform(const UITransform& other)
		: position{other.position},
		  scale{other.scale}
	{
	}

	UITransform(UITransform&& other)
		: position{std::move(other.position)},
		  scale{std::move(other.scale)}
	{
	}

	UITransform& operator=(UITransform other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	UITransform()
		: position(), scale(1,1)
	{}

	explicit UITransform(const sf::Vector2f& pos)
		: position(pos), scale(1,1)
	{}

	UITransform(const sf::Vector2f& pos, const sf::Vector2f& scaling)
		: position(pos), scale(scaling)
	{}

	sf::Vector2f position;
	sf::Vector2f scale;
};