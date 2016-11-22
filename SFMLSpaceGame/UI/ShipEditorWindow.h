#pragma once
#include <UI/GameWindow.h>
#include <SFGUI/Widgets.hpp>
#include <ShipStats.h>
#include <Serializer.h>
#include <SFML/Graphics/Sprite.hpp>

class ShipEditorWindow : public GameWindow
{
private:
	Serializer<> serializer;

	sfg::Button::Ptr m_newShipButton;
	sfg::Button::Ptr m_editShipButton;
	sfg::Button::Ptr m_saveShipButton;
	sfg::Button::Ptr m_deleteShipButton;
	sfg::Button::Ptr m_defineColliderButton;
	sfg::Button::Ptr m_hardpointEditorButton;
	sfg::Button::Ptr m_thrusterEditorButton;
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

	sfg::Entry::Ptr m_hullStrengthEntry;
	sfg::Entry::Ptr m_frontShieldEntry;
	sfg::Entry::Ptr m_sideShieldEntry;
	sfg::Entry::Ptr m_rearShieldEntry;
	sfg::Entry::Ptr m_shieldRegenEntry;

	std::shared_ptr<sf::Texture> m_shipTexture;
	sf::Sprite m_shipImage;

	void SetupButtonSignals();
	void SetupPropertyTable();
	void SetupCanvasSignals();
	void SetupEntryValidationSignals();
	void SetupEntryFocusSignals();

	void LoadShipStatsToEntries();
	void LoadShipImage();
	bool CheckAllEntryValidity();

	void CreateNewShip();
	void ClearShipEditing();

	void DrawShipCanvas();
	void OnShipSelected(const std::string& name);
	void OnNewShipImageSelected(const std::string& name);
	void OnNewShipNameSelected(const std::string& name);
	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
	void OnSaveShip();
	void OnDeleteShip();

	std::string m_newShipImageName;
	std::string m_shipName;
	std::shared_ptr<ShipStats> m_editingStats;
	std::shared_ptr<ShipStats> m_targetStats;
public:
	ShipEditorWindow();
};