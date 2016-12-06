// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <MainMenuState.h>
#include <GameStateManager.h>
#include <InitialGameState.h>

MainMenuState::MainMenuState()
{
}

void MainMenuState::Init()
{
	m_mainMenu.Show(true);
	m_mainMenu.CenterOnScreen();

	m_mainMenu.SetStartGameCallback([] { GAME_STATE_MANAGER->SetNextState(std::make_unique<InitialGameState>()); });
	m_mainMenu.SetExitCallback([] { GAME_WINDOW->close(); });
}

void MainMenuState::CleanUp()
{
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessEvent(const sf::Event& event) const
{
}

void MainMenuState::Update()
{
	GameWindow::UpdateAllWindows();
}

void MainMenuState::Render(sf::RenderTarget& target)
{
	// For some reason, if this state does not render
	// anything directly to the screen, UI elements 
	// will not be displayed correctly, so we render
	// a single pixel. Must be a bug in SFGUI
	static sf::Vertex v{{1,1}};
	target.draw(&v, 1, sf::PrimitiveType::Points);
}