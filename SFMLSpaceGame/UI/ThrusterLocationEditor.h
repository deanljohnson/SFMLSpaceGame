#pragma once
#include "GameWindow.h"
#include <ShipStats.h>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFGUI/Canvas.hpp>
#include <Serializer.h>

class ThrusterLocationEditor : public GameWindow
{
private:
	Serializer m_serializer;

	sfg::Canvas::Ptr m_shipCanvas;

	std::shared_ptr<ShipStats> m_targetStats;
	std::shared_ptr<sf::Texture> m_shipTexture;
	std::string m_shipName;
	sf::Sprite m_shipImage;

	std::vector<sf::Vector2f> m_locations;
	sf::ConvexShape m_thrusterDisplayShape;

	void SetupCanvasSignals();

	void LoadShipImage();

	void DrawCanvas();

	void OnCanvasLeftClick();
	void AddThruster(sf::Vector2f pos);

	void OnReset();
	void OnSave();
	void OnClose();
public:
	ThrusterLocationEditor();

	void SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats);
};