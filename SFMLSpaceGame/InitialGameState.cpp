#include "stdafx.h"
#include <InitialGameState.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <resource.h>
#include <ContactFilter.h>
#include "PlayerData.h"
#include <RenderBatch.h>

//#include <LaserRig.h>
void TestRun()
{
	/*LaserRig* r = new LaserRig();
	r->name = "Cannon-One";
	r->fireRate = .1f;
	r->heatLimit = 10.0;
	r->cooldownRate = 3.0;
	r->heatGenerated = 1.0;

	Serializer<> ser;
	ser.Save<LaserRig>(r, "Cannon-One", "Cannon-One");*/
}

void AddEnemy()
{
	EntityFactory::CreateShip("Human-Fighter", b2Vec2(5, 5));
}

void InitialGameState::Init()
{
	// Sometimes I need to run some test code at the
	// beginning of the game for implementing new features and
	// such, TestRun is the function to handle that
	TestRun();

	PlayerData::SetActive(std::make_shared<PlayerData>("Alien-One"));

	PlayerData::GetActive()->SetShipChangeCallback(
		[this](const std::string& newName) {
		auto playerHandle = EntityManager::Get(PlayerData::GetActive()->GetID());
		auto playerPos = playerHandle->GetComponent<Position>();

		auto playerSpawner = EntityManager::Get(m_playerSpawnerID);
		auto spawnerPos = &playerSpawner->GetComponent<Position>();
		spawnerPos->position = playerPos.position;

		playerHandle->Destroy();
	});

	m_contactListener = ContactFilter();
	world.SetContactFilter(&m_contactListener);

	/*
	auto playerID = EntityFactory::CreatePlayer();

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

	EntityFactory::CreateStation("Human-Spacestation", b2Vec2(7, 0));
	EntityFactory::CreateStation("Human-Spacestation", b2Vec2(40, 0));
	EntityFactory::CreateStation("Human-Spacestation", b2Vec2(7, 40));
	EntityFactory::CreateStation("Human-Spacestation", b2Vec2(40, 40));

	EntityFactory::CreateAsteroid(b2Vec2(20, 20));

	EntityFactory::CreateSpawner(5.f, "Human-Fighter", b2Vec2(8.f, 8.f));
	m_playerSpawnerID = EntityFactory::CreatePlayerSpawner(b2Vec2(0.f, 0.f));
	
	*/
	auto entMan = new EntityManager();
	auto ser = new Serializer<>();
	//ser->Save<EntityManager>(entMan, "all", "EntMan");
	ser->Load<EntityManager>("all");

	m_shieldStateDisplay.SetTarget(PlayerData::GetActive()->GetID());

	m_shipEditor.Show(false);
	m_rigEditor.Show(false);
	m_rigSelector.Show(false);
	m_rigTypeSelector.Show(false);
	m_rigNameEntry.Show(false);
	m_shipSelector.Show(false);
	m_imageSelector.Show(false);
	m_shipNameEntry.Show(false);
	m_stationWindow.Show(false);
	m_stationTradeWindow.Show(false);
	m_hardPointEditor.Show(false);
	m_colliderEditor.Show(false);
	m_thrusterLocationEditor.Show(false);
	m_confirmationDialog.Show(false);
	m_inventoryWindow.Show(false);
	m_shieldStateDisplay.Show(true);
	m_shipEditor.CenterOnScreen();
	m_rigEditor.CenterOnScreen();
	m_rigSelector.CenterOnScreen();
	m_rigTypeSelector.CenterOnScreen();
	m_rigNameEntry.CenterOnScreen();
	m_shipSelector.CenterOnScreen();
	m_imageSelector.CenterOnScreen();
	m_shipNameEntry.CenterOnScreen();
	m_stationWindow.CenterOnScreen();
	m_stationTradeWindow.CenterOnScreen();
	m_hardPointEditor.CenterOnScreen();
	m_colliderEditor.CenterOnScreen();
	m_thrusterLocationEditor.CenterOnScreen();
	m_confirmationDialog.CenterOnScreen();
	m_inventoryWindow.CenterOnScreen();
	m_shieldStateDisplay.SetPosition(sf::Vector2f(0, 100));
}

void InitialGameState::CleanUp()
{
	EntityManager::Clear();
}

void InitialGameState::Pause()
{
	m_paused = true;
}

void InitialGameState::Resume()
{
	m_paused = false;
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

	GameWindow::UpdateAllWindows();

	HandlePause();
	if (m_paused) return;

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
	RenderBatch::RenderAll(target, rendStates);
}

void InitialGameState::HandlePause() 
{
	bool pausingWindowOpened = false;

	pausingWindowOpened |= m_shipEditor.IsShown();
	pausingWindowOpened |= m_shipSelector.IsShown();
	pausingWindowOpened |= m_shipNameEntry.IsShown();

	pausingWindowOpened |= m_rigEditor.IsShown();
	pausingWindowOpened |= m_rigSelector.IsShown();
	pausingWindowOpened |= m_rigTypeSelector.IsShown();
	pausingWindowOpened |= m_rigNameEntry.IsShown();

	pausingWindowOpened |= m_imageSelector.IsShown();
	pausingWindowOpened |= m_stationWindow.IsShown();
	pausingWindowOpened |= m_stationTradeWindow.IsShown();
	pausingWindowOpened |= m_hardPointEditor.IsShown();
	pausingWindowOpened |= m_colliderEditor.IsShown();
	pausingWindowOpened |= m_thrusterLocationEditor.IsShown();
	pausingWindowOpened |= m_confirmationDialog.IsShown();
	pausingWindowOpened |= m_inventoryWindow.IsShown();

	if (pausingWindowOpened && !m_paused)
		Pause();
	else if (!pausingWindowOpened && m_paused)
		Resume();
}
