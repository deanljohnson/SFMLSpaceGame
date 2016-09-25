#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Position.h"
#include "CirclePrimitive.h"
#include "Time.h"

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity(m_world);

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
	m_stepper.Step(m_world, Time::deltaTime);

	m_entityManager.Refresh();
	m_entityManager.Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	m_entityManager.Render(target);
}
