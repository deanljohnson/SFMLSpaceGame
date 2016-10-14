#pragma once

class GameWindow
{
private:
	static int m_windowsWithMouse;
	bool m_containsMouse;
public:
	~GameWindow();

	static bool MouseInWindow() { return m_windowsWithMouse > 0; }

	void OnMouseEnter();
	void OnMouseLeave();
};