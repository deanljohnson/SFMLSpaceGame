#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/Graphics/RenderWindow.hpp>

const float PIXELS_PER_METER = 100.f;
const float FIXED_TIME_STEP = .017f;
const int BOX2D_VELOCITY_ITERATIONS = 8;
const int BOX2D_POSITION_ITERATIONS = 3;

extern sf::RenderWindow* GAME_WINDOW;

inline b2Vec2 MapSFMLToWorld(const b2Vec2& v)
{
	return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
}

inline b2Vec2 MapSFMLToWorld(const sf::Vector2f& v)
{
	return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
}

inline sf::Vector2f MapWorldToSFML(const b2Vec2& v)
{
	return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
}

inline sf::Vector2f MapWorldToSFML(const sf::Vector2f& v)
{
	return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
}