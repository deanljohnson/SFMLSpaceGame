#include "GameStateManager.h"
#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>

void GameStateManager::Init()
{
	m_states.push(new InitialGameState());
	m_states.top()->Init();
}

void GameStateManager::CleanUp() const
{
}

void GameStateManager::ProcessEvent(const sf::Event& event) const
{
	m_states.top()->ProcessEvent(event);
}

void GameStateManager::Update() const
{
	m_states.top()->Update();
}

void GameStateManager::Render(sf::RenderTarget& target) const
{
	m_states.top()->Render(target);
}

void GameStateManager::ChangeState(GameState* s)
{
	m_states.pop();
	m_states.push(s);
}

void GameStateManager::PushState(GameState* s)
{
	m_states.push(s);
}

void GameStateManager::PopState()
{
	m_states.pop();
}