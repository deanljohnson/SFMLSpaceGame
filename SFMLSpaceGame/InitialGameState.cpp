#include <InitialGameState.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <resource.h>
#include <ContactFilter.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include "UI/UI.h"
#include "UI/UIText.h"
#include "UI/UIExpandingBackground.h"
#include "UI/UIButton.h"
#include "UI/UICenterOn.h"

void AddEnemy()
{
	EntityFactory::CreateShip(SHIP_HUMAN_FIGHTER, b2Vec2(5, 5));
}

void InitialGameState::Init()
{
	m_contactListener = ContactFilter();
	world.SetContactFilter(&m_contactListener);

	auto player = EntityFactory::CreatePlayer();

	EntityFactory::CreateBackground(BGONE_FRONT, player);

	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();

	EntityFactory::CreateStation(STATION_HUMAN_ONE, b2Vec2(7, 0));
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

	/*INIT_AND_DISPLAY(UIExpandingBackground, id[0], UI_BACKGROUND_METAL_NINE, sf::Vector2i(400, 300), UITransform(400, 50));
	CENTER_ON(id[0], id[1],
		MAKE_HIERARCHY(
			INIT_AND_DISPLAY(UIExpandingBackground, id[2], UI_BACKGROUND_METAL_NINE, sf::Vector2i(150, 150), UITransform(-50, 0)),
			INIT_AND_DISPLAY(UIButton, id[3], UI_BUTTON_TEST_ONE, UITransform(20, 20))
		) 
	);*/

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

	m_stepper.Step(world, GameTime::deltaTime);

	EntityManager::Refresh();
	EntityManager::Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	// TODO: I don't like this being here, but it is simple and it works.
	pendingEvents.clear();

	sf::RenderStates rendStates;
	rendStates.transform.scale(PIXELS_PER_METER, PIXELS_PER_METER);
	EntityManager::Render(target, rendStates);

	auto view = GAME_WINDOW->getView();
	GAME_WINDOW->setView(GAME_WINDOW->getDefaultView());

	rendStates = sf::RenderStates::Default;
	UI::Render(target, rendStates);

	GAME_WINDOW->setView(view);
}
