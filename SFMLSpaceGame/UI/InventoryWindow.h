#pragma once
#include "GameWindow.h"
#include <EntityID.h>
#include <EntityHandle.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Table.hpp>

class InventoryWindow : public GameWindow
{
private:
	EntityHandle m_targetHandle;

	sfg::Canvas::Ptr m_shipCanvas;
	sfg::Label::Ptr m_hullStateLabel;
	sfg::Label::Ptr m_creditsLabel;

	sfg::Table::Ptr m_itemTable;

	std::shared_ptr<sf::Texture> m_shipTexture;
	sf::Sprite m_shipImage;

	void SetupItemTable();

	void LoadShipImage();
	void LoadHullState();
	void LoadCredits();

	void DrawShipCanvas();
public:
	InventoryWindow();

	void SetTarget(EntityID id);
};