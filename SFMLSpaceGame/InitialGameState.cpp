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
#include "UI/UIExpandingBackground.h"
#include "UI/UIButton.h"
#include <SFML/Window/Event.hpp>
#include "UI/UICenterOn.h"

void AddEnemy(std::shared_ptr<EntityHandle> player, EntityManager* entMan, b2World* world)
{
	auto enemy = entMan->AddEntity(world, NON_PLAYER_SHIP_GROUP);
	EntityFactory::MakeIntoShip(enemy, SHIP_HUMAN_FIGHTER, b2Vec2(5, 5));

	auto en = *enemy.get();
	auto& sc = en->GetComponent<ShipController>();
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

	auto station = m_entityManager.AddEntity(&m_world, STATION_GROUP);
	EntityFactory::MakeIntoStation(station, STATION_HUMAN_ONE, b2Vec2(7, 0));
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
	if (!UI::HandleEvent(event))
	{
		sf::Event copy = sf::Event(event);
		pendingEvents.push_back(std::make_unique<sf::Event>(copy));
	}
}
UI_ID id[7];

void InitialGameState::Update()
{
	UI::Update();

	INIT_AND_DISPLAY(UIExpandingBackground, id[0], UI_BACKGROUND_METAL_NINE, sf::Vector2i(400, 300), UITransform(400, 50));
	CENTER_ON(id[0], id[1],
		MAKE_HIERARCHY(
			INIT_AND_DISPLAY(UIExpandingBackground, id[2], UI_BACKGROUND_METAL_NINE, sf::Vector2i(150, 150), UITransform(-50, 0)),
			INIT_AND_DISPLAY(UIButton, id[3], UI_BUTTON_TEST_ONE, UITransform(20, 20))
		) 
	);


	/*MAKE_HIERARCHY(
		INIT_AND_DISPLAY(UIExpandingBackground, id[0], UI_BACKGROUND_METAL_NINE, sf::Vector2i(1000, 500), UITransform(50, 50)),
		MAKE_HIERARCHY(
			INIT_AND_DISPLAY(UIExpandingBackground, id[3], UI_BACKGROUND_METAL_NINE, sf::Vector2i(200, 100), UITransform(20, 20)),
			VERTICAL_GROUP( id[6],
				INIT_AND_DISPLAY(UIButton, id[4], UI_BUTTON_TEST_ONE, UITransform(20, 20)),
				INIT_AND_DISPLAY(UIButton, id[5], UI_BUTTON_TEST_ONE, UITransform(20, 0))
			)
		)
	);*/

	m_stepper.Step(m_world, GameTime::deltaTime);

	m_entityManager.Refresh();
	m_entityManager.Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	// TODO: I don't like this being here, but it is simple and it works.
	pendingEvents.clear();

	sf::RenderStates rendStates;
	rendStates.transform.scale(PIXELS_PER_METER, PIXELS_PER_METER);
	m_entityManager.Render(target, rendStates);

	auto view = GAME_WINDOW->getView();
	GAME_WINDOW->setView(GAME_WINDOW->getDefaultView());

	rendStates = sf::RenderStates::Default;
	UI::Render(target, rendStates);

	GAME_WINDOW->setView(view);
}
