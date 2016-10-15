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

class ShipEditorWindow : public GameWindow
{
private:
	Serializer serializer;

	sfg::Window::Ptr m_window;
	sfg::Button::Ptr m_newShipButton;
	sfg::Button::Ptr m_editShipButton;
	sfg::Button::Ptr m_saveShipButton;
	sfg::Window::Ptr m_shipWindow;
	sfg::Canvas::Ptr m_shipCanvas;
	sfg::Table::Ptr m_propertyTable;

	// Alignment controls
	sfg::Box::Ptr m_topLevelBox;
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

	void SetupWindowSignals();
	void SetupButtonSignals();
	void SetupPropertyTable();
	void SetupEntryValidationSignals();

	void LoadShipStatsToEntries();
	bool CheckAllEntryValidity();

	void OnShipSelected(const std::string& name);
	void OnEntryFloatTextValidation(sfg::Entry::Ptr entry);
	void OnSaveShip();

	std::string m_originalName;
	std::unique_ptr<ShipStats> m_editingStats;
	std::shared_ptr<ShipStats> m_targetStats;
public:
	ShipEditorWindow();

	virtual void Show(bool val = true) override;
	virtual bool IsShown() override;
};