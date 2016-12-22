// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <InitialGameState.h>
#include <GameTime.h>
#include <EntityFactory.h>
#include <EntityManager.h>
#include <resource.h>
#include <ContactFilter.h>
#include "PlayerData.h"
#include <RenderBatch.h>
#include <Components/Position.h>
#include <Serializer.h>
#include <Entity.h>

#include <UI/ShipEditorWindow.h>
#include <UI/RigEditor.h>
#include <UI/RigSelector.h>
#include <UI/RigTypeSelector.h>
#include <UI/RigNameEntry.h>
#include <UI/ShipSelector.h>
#include <UI/ImageSelector.h>
#include <UI/ShipNameEntry.h>
#include <UI/StationWindow.h>
#include <UI/StationTradeWindow.h>
#include <UI/HardPointEditor.h>
#include <UI/ColliderEditor.h>
#include <UI/ThrusterLocationEditor.h>
#include <UI/ConfirmationDialog.h>
#include <UI/ShieldStateDisplay.h>
#include <UI/InventoryWindow.h>
#include <UI/ContextMenu.h>

#include <WorldDefinition.h>
#include <Economy.h>
#include <GameView.h>

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

class InitialGameState::InitialGameStateImpl
{
public:
	ShipEditorWindow m_shipEditor;
	RigEditor m_rigEditor;
	RigSelector m_rigSelector;
	RigTypeSelector m_rigTypeSelector;
	RigNameEntry m_rigNameEntry;
	ShipSelector m_shipSelector;
	ImageSelector m_imageSelector;
	ShipNameEntry m_shipNameEntry;
	StationWindow m_stationWindow;
	StationTradeWindow m_stationTradeWindow;
	HardPointEditor m_hardPointEditor;
	ColliderEditor m_colliderEditor;
	ThrusterLocationEditor m_thrusterLocationEditor;
	ConfirmationDialog m_confirmationDialog;
	InventoryWindow m_inventoryWindow;
	ContextMenu m_contextMenu;

	ShieldStateDisplay m_shieldStateDisplay;
};

void AddEnemy()
{
	EntityFactory::CreateShip("Human-Fighter", b2Vec2(5, 5));
}

InitialGameState::InitialGameState() 
	: m_playerID(ENTITY_ID_NULL),
	  m_playerSpawnerID(ENTITY_ID_NULL),
	  m_stepper(),
	  m_impl(new InitialGameStateImpl()),
	  m_paused(false)
{}

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
		auto& playerPos = playerHandle->GetComponent<Position>();

		auto playerSpawner = EntityManager::Get(m_playerSpawnerID);
		auto spawnerPos = &playerSpawner->GetComponent<Position>();
		spawnerPos->position = playerPos.position;

		playerHandle->Destroy();
	});

	m_contactListener = ContactFilter();
	world.SetContactFilter(&m_contactListener);

	if (false)
	{
		auto ser = Serializer<>();

		ser.Load<EntityManager>("all");
	}
	else
	{
		auto ser = Serializer<>();
		ser.Load<WorldDefinition>("starter-world");

		EntityFactory::CreatePlayer();

		EntityFactory::CreateMusicPlayer(MUSIC_ONE);
		EntityFactory::CreateBackground(BGONE_FRONT, PlayerData::GetActive()->GetID());

		EntityFactory::CreateSpawner(5.f, "Human-Fighter", b2Vec2(8.f, 8.f));
		m_playerSpawnerID = EntityFactory::CreatePlayerSpawner(b2Vec2(0.f, 0.f));
		

		auto entMan = new EntityManager();
		
		ser.Save<EntityManager>(entMan, "all", "EntMan");
	}

	m_impl->m_shieldStateDisplay.SetTarget(PlayerData::GetActive()->GetID());

	m_impl->m_shipEditor.Show(false);
	m_impl->m_rigEditor.Show(false);
	m_impl->m_rigSelector.Show(false);
	m_impl->m_rigTypeSelector.Show(false);
	m_impl->m_rigNameEntry.Show(false);
	m_impl->m_shipSelector.Show(false);
	m_impl->m_imageSelector.Show(false);
	m_impl->m_shipNameEntry.Show(false);
	m_impl->m_stationWindow.Show(false);
	m_impl->m_stationTradeWindow.Show(false);
	m_impl->m_hardPointEditor.Show(false);
	m_impl->m_colliderEditor.Show(false);
	m_impl->m_thrusterLocationEditor.Show(false);
	m_impl->m_confirmationDialog.Show(false);
	m_impl->m_inventoryWindow.Show(false);
	m_impl->m_contextMenu.Show(false);
	m_impl->m_shieldStateDisplay.Show(true);
	m_impl->m_shipEditor.CenterOnScreen();
	m_impl->m_rigEditor.CenterOnScreen();
	m_impl->m_rigSelector.CenterOnScreen();
	m_impl->m_rigTypeSelector.CenterOnScreen();
	m_impl->m_rigNameEntry.CenterOnScreen();
	m_impl->m_shipSelector.CenterOnScreen();
	m_impl->m_imageSelector.CenterOnScreen();
	m_impl->m_shipNameEntry.CenterOnScreen();
	m_impl->m_stationWindow.CenterOnScreen();
	m_impl->m_stationTradeWindow.CenterOnScreen();
	m_impl->m_hardPointEditor.CenterOnScreen();
	m_impl->m_colliderEditor.CenterOnScreen();
	m_impl->m_thrusterLocationEditor.CenterOnScreen();
	m_impl->m_confirmationDialog.CenterOnScreen();
	m_impl->m_inventoryWindow.CenterOnScreen();
	m_impl->m_contextMenu.CenterOnScreen();
	m_impl->m_shieldStateDisplay.SetPosition(sf::Vector2f(0, 100));
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

	// Update UI
	GameWindow::UpdateAllWindows();

	HandlePause();
	if (m_paused) return;

	pendingGameEvents.Update();
	
	// Update physics
	m_stepper.Step(world, GameTime::deltaTime);

	Economy::Update();

	// Update entities
	EntityManager::Refresh();
	EntityManager::Update();
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	// TODO: I don't like this being here, but it is simple and it works.
	pendingEvents.clear();

	sf::RenderStates rendStates;
	rendStates.transform.scale(GameView::PIXELS_PER_METER, GameView::PIXELS_PER_METER);
	
	EntityManager::Render(target, rendStates);
	RenderBatch::RenderAll(target, rendStates);
}

void InitialGameState::HandlePause() 
{
	bool pausingWindowOpened = false;

	pausingWindowOpened |= m_impl->m_shipEditor.IsShown();
	pausingWindowOpened |= m_impl->m_shipSelector.IsShown();
	pausingWindowOpened |= m_impl->m_shipNameEntry.IsShown();

	pausingWindowOpened |= m_impl->m_rigEditor.IsShown();
	pausingWindowOpened |= m_impl->m_rigSelector.IsShown();
	pausingWindowOpened |= m_impl->m_rigTypeSelector.IsShown();
	pausingWindowOpened |= m_impl->m_rigNameEntry.IsShown();

	pausingWindowOpened |= m_impl->m_imageSelector.IsShown();
	pausingWindowOpened |= m_impl->m_stationWindow.IsShown();
	pausingWindowOpened |= m_impl->m_stationTradeWindow.IsShown();
	pausingWindowOpened |= m_impl->m_hardPointEditor.IsShown();
	pausingWindowOpened |= m_impl->m_colliderEditor.IsShown();
	pausingWindowOpened |= m_impl->m_thrusterLocationEditor.IsShown();
	pausingWindowOpened |= m_impl->m_confirmationDialog.IsShown();
	pausingWindowOpened |= m_impl->m_inventoryWindow.IsShown();

	if (pausingWindowOpened && !m_paused)
		Pause();
	else if (!pausingWindowOpened && m_paused)
		Resume();
}