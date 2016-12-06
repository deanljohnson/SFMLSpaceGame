// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/MainMenu.h>

MainMenu::MainMenu()
	: GameWindow("main_menu")
{
	// dummy window to contain everything
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_window->SetId("main_menu");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto startButton = sfg::Button::Create("Start Game");
	m_window->Add(startButton);

	startButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnStartGame(); });
}

void MainMenu::Update()
{
}

void MainMenu::SetStartGameCallback(std::function<void()> startGameCallback)
{
	m_startGameCallback = startGameCallback;
}

void MainMenu::OnStartGame()
{
	if (m_startGameCallback != nullptr)
		m_startGameCallback();
}