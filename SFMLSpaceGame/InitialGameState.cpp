#include <InitialGameState.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <GameTime.h>
#include <EntityFactory.h>
#include <resource.h>

void InitialGameState::Init()
{
	auto player = m_entityManager.AddEntity(m_world, Group(1));
	EntityFactory::MakeIntoPlayer(player);

	auto bg = m_entityManager.AddEntity(m_world, Group(0));
	EntityFactory::MakeIntoBackgroundOne(bg, player);

	auto enemy = m_entityManager.AddEntity(m_world, Group(1));
	EntityFactory::MakeIntoShip(enemy, SHIP_HUMAN_FIGHTER, b2Vec2(5, 5));
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
