#pragma once
#include "GameWindow.h"
#include <SFGUI/Canvas.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

template<typename TKey>
class TextureMap;
class ShipStats;

class ColliderEditor : public GameWindow
{
private:
	sfg::Canvas::Ptr m_shipCanvas;

	std::shared_ptr<ShipStats> m_targetStats;
	std::shared_ptr<TextureMap<std::string>> m_shipTexture;
	std::string m_shipName;
	sf::Sprite m_shipImage;

	sf::VertexArray m_verts;
	sf::ConvexShape m_fillShape;

	void SetupCanvasSignals();

	void LoadShipImage();

	void DrawCanvas();

	void OnCanvasLeftClick();
	void AddVertex(const sf::Vector2f& pos);

	void OnReset();
	void OnSave();
	void OnClose();
public:
	ColliderEditor();

	void SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats);
};