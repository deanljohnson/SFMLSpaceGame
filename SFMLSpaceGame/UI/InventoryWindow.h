#pragma once
#include <UI/GameWindow.h>
#include <UI/InventoryWidget.h>
#include <EntityID.h>
#include <EntityHandle.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Label.hpp>

class InventoryWindow : public GameWindow
{
private:
	EntityHandle m_targetHandle;

	sfg::Canvas::Ptr m_shipCanvas;
	sfg::Label::Ptr m_hullStateLabel;
	sfg::Label::Ptr m_creditsLabel;

	InventoryWidget m_invenWidget;
	std::shared_ptr<ItemContextProvider> m_contextProvider;

	std::shared_ptr<sf::Texture> m_shipTexture;
	sf::Sprite m_shipImage;

	void LoadShipImage();
	void LoadHullState();
	void LoadCredits();

	void DrawShipCanvas();
public:
	InventoryWindow();

	void SetTarget(EntityID id);
};