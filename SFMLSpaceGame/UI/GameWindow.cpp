#include <UI/GameWindow.h>

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

GameWindow* GameWindow::GetWindow(std::string id)
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