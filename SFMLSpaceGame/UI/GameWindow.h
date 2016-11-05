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
	explicit GameWindow(const std::string& id);
	virtual ~GameWindow();

	static bool MouseInWindow();

	template<typename T>
	static T* GetWindow(const std::string& id);
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

template <typename T>
T* GameWindow::GetWindow(const std::string& id)
{
	return static_cast<T*>(GetWindow(id));
}