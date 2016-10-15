#pragma once
#include <string>
#include <map>

class GameWindow
{
protected:
	static int m_windowsWithMouse;
	static std::map<std::string, GameWindow*> m_windows;

	bool m_containsMouse;
	std::string m_id;
public:
	explicit GameWindow(std::string id);
	virtual ~GameWindow();

	static bool MouseInWindow() { return m_windowsWithMouse > 0; }
	static GameWindow* GetWindow(std::string id);

	void OnMouseEnter();
	void OnMouseLeave();

	virtual void Show(bool val = true) = 0;
	virtual bool IsShown() = 0;
};