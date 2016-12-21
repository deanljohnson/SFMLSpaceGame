// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <GameView.h>

namespace GameView
{
	sf::RenderWindow* GAME_WINDOW;

	b2Vec2 MapPixelToWorld(const b2Vec2& v)
	{
		return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
	}

	b2Vec2 MapPixelToWorld(const sf::Vector2f& v)
	{
		return b2Vec2(v.x / PIXELS_PER_METER, v.y / PIXELS_PER_METER);
	}

	sf::Vector2f MapWorldToPixel(const b2Vec2& v)
	{
		return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
	}

	sf::Vector2f MapWorldToPixel(const sf::Vector2f& v)
	{
		return sf::Vector2f(v.x * PIXELS_PER_METER, v.y * PIXELS_PER_METER);
	}

	b2Vec2 GetWorldMouseLocation()
	{
		auto sfPos = GAME_WINDOW->mapPixelToCoords(sf::Mouse::getPosition(*GAME_WINDOW));
		return MapPixelToWorld(sfPos); // convert window coordinate value to world (meters) value
	}

	sf::Vector2f GetScreenMouseLocation()
	{
		auto pos = sf::Mouse::getPosition(*GAME_WINDOW);
		return{ static_cast<float>(pos.x), static_cast<float>(pos.y) };
	}

	void ZoomGameView(float zoomAmount)
	{
		auto v = GAME_WINDOW->getView();
		v.zoom(zoomAmount);
		GAME_WINDOW->setView(v);
	}

	void SetGameViewCenter(const b2Vec2& p)
	{
		auto v = GAME_WINDOW->getView();
		v.setCenter(MapWorldToPixel(p));
		GAME_WINDOW->setView(v);
	}

	void CloseWindow()
	{
		GAME_WINDOW->close();
	}

	void SetWindow(sf::RenderWindow* window)
	{
		GAME_WINDOW = window;
	}
}