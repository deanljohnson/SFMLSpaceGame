#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Position.h"
#include "RectPrimitive.h"
#include "Time.h"
#include "DirectionalKeyboardInput.h"
#include "DirectionalVelocity.h"

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity(m_world);

	ent->AddComponent<Position>();
	ent->AddComponent<RectPrimitive, float, float>(50.f, 100.f);
	ent->AddComponent<Physics>();
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<DirectionalVelocity>();
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
