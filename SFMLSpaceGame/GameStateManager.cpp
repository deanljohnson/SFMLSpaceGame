// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <GameStateManager.h>
#include <InitialGameState.h>

void GameStateManager::Init()
{
	m_states.emplace_back(new InitialGameState());
	m_states.back()->Init();
}

void GameStateManager::CleanUp()
{
	while (!m_states.empty())
	{
		m_states.back()->CleanUp();
		m_states.pop_back();
	}
}

void GameStateManager::ProcessEvent(const sf::Event& event) const
{
	m_states.back()->ProcessEvent(event);
}

void GameStateManager::Update() const
{
	m_states.back()->Update();
}

void GameStateManager::Render(sf::RenderTarget& target) const
{
	m_states.back()->Render(target);
}

void GameStateManager::ChangeState(GameState* s)
{
	m_states.pop_back();
	m_states.emplace_back(s);
}

void GameStateManager::PushState(GameState* s)
{
	m_states.emplace_back(s);
}