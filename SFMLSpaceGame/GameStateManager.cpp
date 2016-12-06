// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <GameStateManager.h>
#include <InitialGameState.h>
#include <MainMenuState.h>

void GameStateManager::Init()
{
	PushState(std::make_unique<MainMenuState>());
}

void GameStateManager::CleanUp()
{
	while (!m_states.empty())
	{
		PopState();
	}
}

void GameStateManager::ProcessEvent(const sf::Event& event) const
{
	m_states.back()->ProcessEvent(event);
}

void GameStateManager::Update()
{
	if (m_nextState != nullptr)
	{
		ChangeState(move(m_nextState));
		m_nextState.release();
	}
	m_states.back()->Update();
}

void GameStateManager::Render(sf::RenderTarget& target) const
{
	m_states.back()->Render(target);
}

void GameStateManager::SetNextState(std::unique_ptr<GameState> s)
{
	m_nextState = move(s);
}

void GameStateManager::ChangeState(std::unique_ptr<GameState> s)
{
	PopState();
	PushState(move(s));
}

void GameStateManager::PushState(std::unique_ptr<GameState> s)
{
	m_states.emplace_back(move(s));
	m_states.back()->Init();
}

void GameStateManager::PopState()
{
	m_states.back()->CleanUp();
	m_states.pop_back();
}