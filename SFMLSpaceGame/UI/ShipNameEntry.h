#pragma once
#include "GameWindow.h"
#include <SFGUI/Window.hpp>

class ShipNameEntry : public GameWindow
{
private:
	std::function<void(const std::string&)> m_callback;

	void CallCallback(const std::string& name);
public:
	ShipNameEntry();

	void SetCallback(std::function<void(const std::string&)> callback);
};