#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Position.h"
#include "Rotation.h"
#include "RectPrimitive.h"
#include "Time.h"
#include "DirectionalKeyboardInput.h"
#include "DirectionalVelocity.h"
#include "RotatetoFaceMouse.h"

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity(m_world);

	ent->AddComponent<Position>();
	ent->AddComponent<Rotation>();
	auto rp = ent->AddComponent<RectPrimitive, float, float>(100.f, 50.f);
	auto phys = ent->AddComponent<Physics>();
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<DirectionalVelocity, float>(5.f);
	ent->AddComponent<RotateToFaceMouse>();

	phys.AddShape(rp.GetShape());
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
