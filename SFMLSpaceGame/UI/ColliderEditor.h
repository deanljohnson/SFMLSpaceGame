#pragma once
#include "GameWindow.h"
#include <Serializer.h>
#include <SFGUI/Canvas.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <ShipStats.h>
#include <SFML/Graphics/ConvexShape.hpp>

class ColliderEditor : public GameWindow
{
private:
	Serializer<> m_serializer;

	sfg::Canvas::Ptr m_shipCanvas;

	std::shared_ptr<ShipStats> m_targetStats;
	std::shared_ptr<sf::Texture> m_shipTexture;
	std::string m_shipName;
	sf::Sprite m_shipImage;

	sf::VertexArray m_verts;
	sf::ConvexShape m_fillShape;

	void SetupCanvasSignals();

	void LoadShipImage();

	void DrawCanvas();

	void OnCanvasLeftClick();
	void AddVertex(sf::Vector2f pos);

	void OnReset();
	void OnSave();
	void OnClose();
public:
	ColliderEditor();

	void SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats);
};