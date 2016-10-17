#include <UI/GameWindow.h>
#include "UI.h"

int GameWindow::m_windowsWithMouse = 0;
std::map<std::string, GameWindow*> GameWindow::m_windows{};

GameWindow::GameWindow(std::string id)
	: m_id(id)
{
	m_windows.emplace(m_id, this);
}

GameWindow::~GameWindow()
{
	if (m_containsMouse)
		m_windowsWithMouse++;
}

GameWindow* GameWindow::GetWindow(const std::string& id)
{
	auto it = m_windows.find(id);
	if (it != m_windows.end())
		return it->second;

	return nullptr;
}

void GameWindow::OnMouseEnter()
{
	m_windowsWithMouse++;
	m_containsMouse = true;
}

void GameWindow::OnMouseLeave()
{
	m_windowsWithMouse--;
	m_containsMouse = false;

	if (m_windowsWithMouse < 0) m_windowsWithMouse = 0;
}

void GameWindow::OnMouseMove()
{
	if (!m_containsMouse)
	{
		OnMouseEnter();
	}
}

void GameWindow::SetupWindowSignals()
{
	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect([this] { OnMouseEnter(); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect([this] { OnMouseLeave(); });
	m_window->GetSignal(sfg::Window::OnMouseMove).Connect([this] { OnMouseMove(); });
	m_window->GetSignal(sfg::Window::OnCloseButton).Connect([this] { Show(false); });
}

void GameWindow::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
	{
		if (val)
			UI::Singleton->BringToFront(m_window);
		return;
	}

	m_window->Show(val);

	if (val)
		UI::Singleton->BringToFront(m_window);

	if (m_containsMouse && !val)
		OnMouseLeave();
}

bool GameWindow::IsShown()
{
	return m_window->IsLocallyVisible();
}

void GameWindow::CenterOnScreen()
{
	// Center window on screen
	m_window->SetPosition(
		sf::Vector2f(
		(UI::Singleton->GetSize().x / 2.f) - m_window->GetAllocation().width / 2.f,
		(UI::Singleton->GetSize().y / 2.f) - m_window->GetAllocation().height / 2.f));

}