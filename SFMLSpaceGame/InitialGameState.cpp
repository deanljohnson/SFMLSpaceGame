#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Position.h"
#include "CirclePrimitive.h"

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity();

	ent->AddComponent<Position>();
	ent->AddComponent<CirclePrimitive, float>(100.f);
}

void InitialGameState::CleanUp() const
{
	
}

void InitialGameState::Pause() const
{
	
}

void InitialGameState::Resume() const
{
	
}

void InitialGameState::ProcessEvent(const sf::Event& event) const
{
	
}

void InitialGameState::Update()
{
	m_entityManager.Refresh();
	m_entityManager.Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	m_entityManager.Render(target);
}
