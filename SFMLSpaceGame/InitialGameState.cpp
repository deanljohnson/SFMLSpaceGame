#include <InitialGameState.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <resource.h>
#include <ContactFilter.h>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include "PlayerData.h"

void AddEnemy()
{
	EntityFactory::CreateShip("Human-Fighter", b2Vec2(5, 5));
}

void InitialGameState::Init()
{
	PlayerData::SetActive(std::make_shared<PlayerData>("Human-Fighter"));

	m_contactListener = ContactFilter();
	world.SetContactFilter(&m_contactListener);

	auto playerID = EntityFactory::CreatePlayer();

	PlayerData::GetActive()->SetShipChangeCallback(
		[this](const std::string& newName){
			auto playerHandle = EntityManager::Get(PlayerData::GetActive()->GetID());
			auto playerPos = playerHandle->GetComponent<Position>();

			auto playerSpawner = EntityManager::Get(m_playerSpawnerID);
			auto spawnerPos = &playerSpawner->GetComponent<Position>();
			spawnerPos->position = playerPos.position;

			playerHandle->Destroy();
		});

	EntityFactory::CreateMusicPlayer(MUSIC_ONE);
	EntityFactory::CreateBackground(BGONE_FRONT, playerID);

	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();
	AddEnemy();

	EntityFactory::CreateStation(STATION_HUMAN_ONE, b2Vec2(7, 0));
	EntityFactory::CreateStation(STATION_HUMAN_ONE, b2Vec2(40, 0));
	EntityFactory::CreateStation(STATION_HUMAN_ONE, b2Vec2(7, 40));
	EntityFactory::CreateStation(STATION_HUMAN_ONE, b2Vec2(40, 40));

	EntityFactory::CreateSpawner(5.f, SHIP_HUMAN_FIGHTER, b2Vec2(8.f, 8.f));
	m_playerSpawnerID = EntityFactory::CreatePlayerSpawner(b2Vec2(0.f, 0.f));

	m_shipEditor.Show(false);
	m_shipSelector.Show(false);
	m_imageSelector.Show(false);
	m_shipNameEntry.Show(false);
	m_stationWindow.Show(false);
	m_hardPointEditor.Show(false);
	m_colliderEditor.Show(false);
	m_thrusterLocationEditor.Show(false);
	m_shipEditor.CenterOnScreen();
	m_shipSelector.CenterOnScreen();
	m_imageSelector.CenterOnScreen();
	m_shipNameEntry.CenterOnScreen();
	m_stationWindow.CenterOnScreen();
	m_hardPointEditor.CenterOnScreen();
	m_colliderEditor.CenterOnScreen();
	m_thrusterLocationEditor.CenterOnScreen();
}

void InitialGameState::CleanUp()
{
	EntityManager::Clear();
}

void InitialGameState::Pause() const
{
	
}

void InitialGameState::Resume() const
{
	
}

void InitialGameState::ProcessEvent(const sf::Event& event) const
{
	sf::Event copy = sf::Event(event);
	pendingEvents.push_back(std::make_unique<sf::Event>(copy));
}

namespace
{
	float m_unusedResourcesTimer = 0.f;
	float m_unusedResourcesTimerLimit = 1.f;
}

void InitialGameState::Update()
{
	m_unusedResourcesTimer += GameTime::deltaTime;
	if (m_unusedResourcesTimer > m_unusedResourcesTimerLimit)
	{
		m_unusedResourcesTimer = 0.f;
		UnloadUnusedResources();
	}

	pendingGameEvents.Update();

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

	GAME_WINDOW->setView(view);
}
