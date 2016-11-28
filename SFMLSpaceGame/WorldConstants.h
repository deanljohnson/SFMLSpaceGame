#pragma once
#include <Box2D/Common/b2Math.h>
#include <SFML/Graphics/RenderWindow.hpp>

const float PIXELS_PER_METER = 100.f;
const float METERS_PER_PIXEL = 1.f / 100.f;
const float FIXED_TIME_STEP = .017f;
const int BOX2D_VELOCITY_ITERATIONS = 8;
const int BOX2D_POSITION_ITERATIONS = 3;

extern size_t DRAW_CALLS;
extern sf::RenderWindow* GAME_WINDOW;

inline b2Vec2 MapPixelToWorld(const b2Vec2& v)
{
	return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
}

inline b2Vec2 MapPixelToWorld(const sf::Vector2f& v)
{
	return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
}

inline sf::Vector2f MapWorldToPixel(const b2Vec2& v)
{
	return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
}

inline sf::Vector2f MapWorldToPixel(const sf::Vector2f& v)
{
	return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
}

inline b2Vec2 GetWorldMouseLocation()
{
	auto sfPos = GAME_WINDOW->mapPixelToCoords(sf::Mouse::getPosition(*GAME_WINDOW));
	return MapPixelToWorld(sfPos); // convert window coordinate value to world (meters) value
}

inline sf::Vector2f GetScreenMouseLocation() 
{
	auto pos = sf::Mouse::getPosition(*GAME_WINDOW);
	return { (float)pos.x, (float)pos.y };
}