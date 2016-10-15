#pragma once
#include "GameWindow.h"
#include <SFGUI/Window.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/ComboBox.hpp>

class ShipSelector : public GameWindow
{
private:
	sfg::Window::Ptr m_window;

	sfg::Table::Ptr m_table;
	sfg::ComboBox::Ptr m_selectionBox;

	std::function<void(const std::string&)> m_callback;

	void SetupWindowSignals();
	void CallCallback(const std::string& name);
public:
	ShipSelector();

	void SetCallback(std::function<void(const std::string&)> callback);

	virtual void Show(bool val = true) override;
	virtual bool IsShown() override;
};