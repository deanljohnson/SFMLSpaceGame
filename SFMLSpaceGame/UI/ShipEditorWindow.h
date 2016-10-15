#pragma once
#include <UI/GameWindow.h>
#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Canvas.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Table.hpp>
#include <ShipStats.h>

class ShipEditorWindow : public GameWindow
{
private:
	sfg::Window::Ptr m_window;
	sfg::Button::Ptr m_newShipButton;
	sfg::Button::Ptr m_editShipButton;
	sfg::Window::Ptr m_shipWindow;
	sfg::Canvas::Ptr m_shipCanvas;
	sfg::Table::Ptr m_propertyTable;

	// Alignment controls
	sfg::Box::Ptr m_topLevelBox;
	sfg::Box::Ptr m_leftSideBar;
	sfg::Box::Ptr m_rightSideBar;

	void SetupWindowSignals();
	void SetupPropertyTable();

	ShipStats m_stats;
public:
	ShipEditorWindow();

	virtual void Show(bool val = true) override;
	virtual bool IsShown() override;
};