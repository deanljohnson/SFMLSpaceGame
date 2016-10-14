#include <UI/GameWindow.h>

int GameWindow::m_windowsWithMouse = 0;

GameWindow::~GameWindow()
{
	if (m_containsMouse)
		m_windowsWithMouse++;
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
}