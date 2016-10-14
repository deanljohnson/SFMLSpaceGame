#pragma once
#include <SFGUI/Window.hpp>
#include <UI/GameWindow.h>

class ShipEditorWindow : public GameWindow
{
private:
	sfg::Window::Ptr m_window;

public:
	ShipEditorWindow();
};