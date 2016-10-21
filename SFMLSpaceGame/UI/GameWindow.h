#pragma once
#include <string>
#include <map>
#include <SFGUI/Window.hpp>
#include <WorldConstants.h>

class GameWindow
{
protected:
	static int m_elementsWithKeyboardFocus;

	static int m_windowsWithMouse;
	static std::map<std::string, GameWindow*> m_windows;

	bool m_containsMouse;
	std::string m_id;
	sfg::Window::Ptr m_window;

	virtual void SetupWindowSignals();

	void OnMouseEnter();
	void OnMouseLeave();
	void OnMouseMove();

public:
	explicit GameWindow(std::string id);
	virtual ~GameWindow();

	static bool MouseInWindow()
	{
		if (m_windowsWithMouse > 0)
			return true;
		else
		{
			auto mousePosI = sf::Mouse::getPosition(*GAME_WINDOW);
			auto mousePos = sf::Vector2f(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));
			for(auto& kvp : m_windows)
			{
				if (!kvp.second->IsShown())
					continue;

				if (kvp.second->m_window->GetAllocation().contains(mousePos))
					return true;
			}
			return false;
		}
		return m_windowsWithMouse > 0;
	}
	static GameWindow* GetWindow(const std::string& id);
	static void GrabKeyboardFocus();
	static void ReleaseKeyboardFocus();
	static bool WindowHasKeyboardFocus();

	static void UpdateAllWindows();

	virtual void Update() {}

	virtual void Show(bool val = true);
	virtual bool IsShown();

	virtual void CenterOnScreen();
	virtual void SetPosition(const sf::Vector2f& pos);
};