#pragma once
#include "GameWindow.h"
#include <SFGUI/Canvas.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <ShipStats.h>
#include <UI\HardPointWidget.h>
#include <Serializer.h>

class HardPointEditor : public GameWindow
{
private:
	HardPointWidget::Type m_hardPointType;

	Serializer serializer;

	sfg::Canvas::Ptr m_shipCanvas;

	std::shared_ptr<ShipStats> m_targetStats;
	std::shared_ptr<sf::Texture> m_shipTexture;
	std::string m_shipName;
	sf::Sprite m_shipImage;

	std::vector<HardPointWidget> m_hardPointWidgets;

	bool m_mouseDownOnPositionWidget;
	bool m_mouseDownOnAngleWidget;
	int m_mouseDownOnWidget;
	bool m_dragOnCanvas;
	int m_lastClickedWidget{ -1 };

	void SetupCanvasSignals();

	void LoadShipImage();

	void DrawCanvas();
	void CreateHardpoint(const sf::Vector2f& pos);

	void OnCanvasLeftClick();
	void OnCanvasMouseMove();
	void OnCanvasLeftPress();
	void OnCanvasLeftRelease();
	void OnCanvasMouseLeave();

	void OnSelectHardPointType(HardPointWidget::Type type);
	void OnDeleteSelectHardpoint();
	void OnSave();
	void OnClose();
public:
	HardPointEditor();

	void SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats);
};