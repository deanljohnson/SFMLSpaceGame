#pragma once
#include <Box2D/Common/b2Math.h>

class RenderWindow;

namespace GameView
{
	const float PIXELS_PER_METER = 100.f;
	const float METERS_PER_PIXEL = 1.f / PIXELS_PER_METER;

	b2Vec2 MapPixelToWorld(const b2Vec2& v);
	b2Vec2 MapPixelToWorld(const sf::Vector2f& v);
	sf::Vector2f MapWorldToPixel(const b2Vec2& v);
	sf::Vector2f MapWorldToPixel(const sf::Vector2f& v);
	b2Vec2 GetWorldMouseLocation();
	sf::Vector2f GetScreenMouseLocation();

	void ZoomGameView(float zoomAmount);
	void SetGameViewCenter(const b2Vec2& p);
	void CloseWindow();

	void SetWindow(sf::RenderWindow* window);
}