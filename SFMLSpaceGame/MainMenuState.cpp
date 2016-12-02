// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <MainMenuState.h>

MainMenuState::MainMenuState()
{
}

void MainMenuState::Init()
{
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
}