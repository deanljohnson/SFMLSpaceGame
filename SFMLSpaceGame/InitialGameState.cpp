#include <InitialGameState.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/RectPrimitive.h>
#include <GameTime.h>
#include <Components/DirectionalKeyboardInput.h>
#include <Components/DirectionalVelocity.h>
#include <Components/RotatetoFaceMouse.h>
#include <Components/ShipThrusters.h>
#include <Components/ThrusterInput.h>
#include <Components/Background.h>
#include <Components/ParallaxMovement.h>
#include <Components/SmoothCameraFollow.h>
#include <Components/Sprite.h>
#include <Components/GameWorldClickListener.h>
#include <Components/DirectionalGun.h>
#include <Components/FireGunOnClick.h>
#include <resource.h>

void InitialGameState::Init()
{
	auto ent = m_entityManager.AddEntity(m_world, Group(1));

	auto pos = ent->AddComponent<Position>();
	ent->AddComponent<Rotation>();
	auto sp = ent->AddComponent<Sprite, ResourceID>(SHIP_HUMAN_FIGHTER);
	auto phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ShipThrusters, ShipThrust, ShipThrust>(ShipThrust(.08f, .06f, .04f), ShipThrust(7.f, 5.f, 5.f));
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float, float>(.8f, .5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<DirectionalGun>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();

	auto spriteBox = sp.GetPixelLocalBounds();
	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(b2Vec2(5, 5));

	auto bg = m_entityManager.AddEntity(m_world, Group(0));

	bg->AddComponent<Position>();
	bg->AddComponent<Background, ResourceID>(BGONE_FRONT);
	bg->AddComponent<ParallaxMovement, Entity&, float>(*ent, .1f);
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
	m_stepper.Step(m_world, GameTime::deltaTime);

	m_entityManager.Refresh();
	m_entityManager.Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	m_entityManager.Render(target);
}
