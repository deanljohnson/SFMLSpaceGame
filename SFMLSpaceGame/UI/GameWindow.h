#pragma once
#include <string>
#include <map>
#include <SFGUI/Window.hpp>

class GameWindow
{
protected:
	static int m_windowsWithMouse;
	static std::map<std::string, GameWindow*> m_windows;

	bool m_containsMouse;
	std::string m_id;
	sfg::Window::Ptr m_window;

	virtual void SetupWindowSignals();
public:
	explicit GameWindow(std::string id);
	virtual ~GameWindow();

	static bool MouseInWindow() { return m_windowsWithMouse > 0; }
	static GameWindow* GetWindow(const std::string& id);

	void OnMouseEnter();
	void OnMouseLeave();

	virtual void Show(bool val = true);
	virtual bool IsShown();

	virtual void CenterOnScreen();
};