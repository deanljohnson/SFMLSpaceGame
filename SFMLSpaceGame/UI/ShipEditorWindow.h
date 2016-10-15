#pragma once
#include <UI/GameWindow.h>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <ShipStats.h>
#include <SFGUI/Entry.hpp>
#include <Serializer.h>
#include <SFML/Graphics/Sprite.hpp>

class ShipEditorWindow : public GameWindow
{
private:
	Serializer serializer;

	sfg::Window::Ptr m_window;
	sfg::Button::Ptr m_newShipButton;
	sfg::Button::Ptr m_editShipButton;
	sfg::Button::Ptr m_saveShipButton;
	sfg::Button::Ptr m_defineColliderButton;
	sfg::Window::Ptr m_shipWindow;
	sfg::Canvas::Ptr m_shipCanvas;
	sfg::Table::Ptr m_propertyTable;

	// Alignment controls
	sfg::Box::Ptr m_topLevelBox;
	sfg::Box::Ptr m_mainBox;
	sfg::Box::Ptr m_leftSideBar;
	sfg::Box::Ptr m_rightSideBar;

	// Field entries
	sfg::Entry::Ptr m_interLeadEntry;
	sfg::Entry::Ptr m_followDistEntry;
	sfg::Entry::Ptr m_approachDistEntry;
	sfg::Entry::Ptr m_strafeDistEntry;
	sfg::Entry::Ptr m_sensorRangeEntry;

	sfg::Entry::Ptr m_forwardThrustEntry;
	sfg::Entry::Ptr m_sideThrustEntry;
	sfg::Entry::Ptr m_reverseThrustEntry;
	sfg::Entry::Ptr m_steerThrustEntry;

	sfg::Entry::Ptr m_fireRateEntry;
	sfg::Entry::Ptr m_heatLimitEntry;
	sfg::Entry::Ptr m_cooldownRateEntry;
	sfg::Entry::Ptr m_heatGenEntry;

	std::shared_ptr<sf::Texture> m_shipTexture;
	sf::Sprite m_shipImage;

	void SetupWindowSignals();
	void SetupButtonSignals();
	void SetupPropertyTable();
	void SetupCanvasSignals();
	void SetupEntryValidationSignals();

	void BeginDefiningCollider();
	void EndDefiningCollider();

	void LoadShipStatsToEntries();
	void LoadShipImage();
	bool CheckAllEntryValidity();

	void CreateNewShip();

	void DrawShipCanvas();

	void OnCanvasClick();
	void OnShipSelected(const std::string& name);
	void OnNewShipImageSelected(const std::string& name);
	void OnNewShipNameSelected(const std::string& name);
	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
	void OnSaveShip();

	bool m_definingCollider{false};
	sf::VertexArray m_colliderVertices;

	std::string m_newShipImageName;
	std::string m_shipName;
	std::unique_ptr<ShipStats> m_editingStats;
	std::shared_ptr<ShipStats> m_targetStats;
public:
	ShipEditorWindow();

	virtual void Show(bool val = true) override;
	virtual bool IsShown() override;

	void SetPosition(const sf::Vector2f& pos);
};