#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "Position.h"
#include "Rotation.h"
#include "RectPrimitive.h"
#include "Time.h"
#include "DirectionalKeyboardInput.h"
#include "DirectionalVelocity.h"
#include "RotatetoFaceMouse.h"
#include "ShipThrusters.h"
#include "ThrusterInput.h"
#include "Background.h"

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity(m_world, Group(1));

	ent->AddComponent<Position>();
	ent->AddComponent<Rotation>();
	auto rp = ent->AddComponent<RectPrimitive, float, float>(100.f, 50.f);
	auto phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ShipThrusters, ShipThrust, ShipThrust>(ShipThrust(.01f, .001f, .005f), ShipThrust(5.f, 3.f, 3.f));
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float, float>(.1f, 2.f);

	phys.AddShape(rp.GetShape());
	phys.SetPosition(b2Vec2(5, 5));

	auto bg = m_entityManager.AddEntity(m_world, Group(0));

	bg->AddComponent<Position>();
	bg->AddComponent<Background, std::wstring>(L"BGONE_FRONT");
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
