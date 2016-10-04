#include <InitialGameState.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <GameTime.h>
#include <EntityFactory.h>
#include <resource.h>
#include <ContactFilter.h>
#include "Components/ShipController.h"
#include <EntityGroups.h>
#include "UI/UI.h"
#include "UI/UIText.h"

void AddEnemy(Entity* player, EntityManager* entMan, b2World* world)
{
	auto enemy = entMan->AddEntity(world, NON_PLAYER_SHIP_GROUP);
	EntityFactory::MakeIntoShip(enemy, SHIP_HUMAN_FIGHTER, b2Vec2(5, 5));
	auto& sc = enemy->GetComponent<ShipController>();
	sc.Set(StrafeToTargetsRearForAttack);
	sc.Set(FireGunsWhenFacingTarget);
	sc.SetTarget(player);
}

void InitialGameState::Init()
{
	m_contactListener = ContactFilter();
	m_world.SetContactFilter(&m_contactListener);

	auto player = m_entityManager.AddEntity(&m_world, PLAYER_GROUP);
	EntityFactory::MakeIntoPlayer(player);

	auto bg = m_entityManager.AddEntity(&m_world, BACKGROUND_GROUP);
	EntityFactory::MakeIntoBackgroundOne(bg, player);

	AddEnemy(player, &m_entityManager, &m_world);
	/*AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);
	AddEnemy(player, &m_entityManager, &m_world);*/
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
UI_ID id[5];

void InitialGameState::Update()
{
	UI::Update();

	INIT_AND_DISPLAY(UIText, id[0], "hello world!", FONT_ONE);

	/*if (id == 0) id = UI::Display<UIText>(id, "hello world!", FONT_ONE);
	else UI::Display<UIText>(id);*/

	m_stepper.Step(m_world, GameTime::deltaTime);

	m_entityManager.Refresh();
	m_entityManager.Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	sf::RenderStates rendStates;
	rendStates.transform.scale(PIXELS_PER_METER, PIXELS_PER_METER);
	m_entityManager.Render(target, rendStates);

	rendStates = sf::RenderStates::Default;
	auto center = GAME_WINDOW->getView().getCenter();
	rendStates.transform.translate(center); // this puts (0,0) at the center of the screen
	rendStates.transform.translate(-GAME_WINDOW->getView().getSize() / 2.f); // and now we move (0,0) to the top-left
	UI::Render(target, rendStates);
}
