// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/GameWindow.h>
#include "UI.h"
#include <WorldConstants.h>

int GameWindow::m_windowsWithMouse = 0;
int GameWindow::m_elementsWithKeyboardFocus = 0;
std::map<std::string, GameWindow*> GameWindow::m_windows{};

GameWindow::GameWindow(const std::string& id)
	: m_id(id)
{
	m_windows.emplace(m_id, this);
}

GameWindow::~GameWindow()
{
	// Remove the SFGUI window
	UI::Singleton->Remove(m_window);

	// Delete this from the static windows collection
	m_windows.erase(m_windows.find(m_id));

	if (m_containsMouse)
		m_windowsWithMouse--;
}

void GameWindow::UpdateAllWindows() 
{
	for (auto& kvp : m_windows) 
	{
		if (kvp.second->IsShown())
			kvp.second->Update();
	}
}

bool GameWindow::MouseInWindow() 
{
	if (m_windowsWithMouse > 0)
		return true;

	// Get the mouse pos within the window as a Vector2f
	auto mousePosI = sf::Mouse::getPosition(*GAME_WINDOW);
	auto mousePos = sf::Vector2f(static_cast<float>(mousePosI.x), static_cast<float>(mousePosI.y));
	
	for (auto& kvp : m_windows)
	{
		// If the window is hidden it can't contain the mouse
		if (!kvp.second->IsShown())
			continue;

		if (kvp.second->m_window->GetAllocation().contains(mousePos))
			return true;
	}
	return false;
}

GameWindow* GameWindow::GetWindow(const std::string& id)
{
	auto it = m_windows.find(id);
	if (it != m_windows.end())
		return it->second;

	return nullptr;
}

void GameWindow::GrabKeyboardFocus()
{
	m_elementsWithKeyboardFocus++;
}

void GameWindow::ReleaseKeyboardFocus()
{
	m_elementsWithKeyboardFocus--;
}

bool GameWindow::WindowHasKeyboardFocus()
{
	return m_elementsWithKeyboardFocus > 0;
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
	// if the windows state isn't actually changing
	if (m_window->IsLocallyVisible() == val)
	{
		// Showing an already open window just pulls that window to the front
		if (val)
			UI::Singleton->BringToFront(m_window);
		return;
	}

	m_window->Show(val);

	// Make sure the newly shown window is on top
	if (val)
		UI::Singleton->BringToFront(m_window);

	// If the window is now hidden but previously contained the mouse
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

void GameWindow::SetPosition(const sf::Vector2f& pos) 
{
	m_window->SetPosition(pos);
}