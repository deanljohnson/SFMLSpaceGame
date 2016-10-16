#pragma once
#include "GameWindow.h"
#include <SFGUI/Window.hpp>
#include <SFGUI/ComboBox.hpp>

class ShipSelector : public GameWindow
{
private:
	sfg::ComboBox::Ptr m_selectionBox;

	std::function<void(const std::string&)> m_callback;

	void CallCallback(const std::string& name);
public:
	ShipSelector();

	void SetCallback(std::function<void(const std::string&)> callback);

	virtual void Show(bool val = true) override;
};