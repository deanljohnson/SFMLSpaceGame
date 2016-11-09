#include "stdafx.h"
#include <resource.h>
#include "EntityManager.h"
#include <EntityGroups.h>
#include <EntityFactory.h>
#include <EntityHandle.h>
#include "CollisionGroups.h"
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/RectPrimitive.h>
#include <Components/DirectionalKeyboardInput.h>
#include <Components/RotatetoFaceMouse.h>
#include <Components/ShipThrusters.h>
#include <Components/ThrusterInput.h>
#include <Components/ParallaxMovement.h>
#include <Components/SmoothCameraFollow.h>
#include <Components/Sprite.h>
#include <Components/GameWorldClickListener.h>
#include <Components/DirectionalGun.h>
#include <Components/FireGunOnClick.h>
#include <Components/BulletPhysics.h>
#include <Components/Lifetime.h>
#include "Components/CollisionFilterComponent.h"
#include "Components/ShipController.h"
#include "Components/EntitySensor.h"
#include "Components/ZoomHandler.h"
#include "Components/ShipAI.h"
#include "Components/Health.h"
#include "Components/DamageOnAttacked.h"
#include "Components/ShipSpawner.h"
#include "Components/PlayerDeathBroadcaster.h"
#include "Components/AnimatedSprite.h"
#include "Components/ThrusterAnimator.h"
#include "Components/SoundSource.h"
#include "Components/SoundListener.h"
#include "Components/MusicSource.h"
#include "Components/GunHeatUIDisplay.h"
#include "Components/TilingBackground.h"
#include "Components/KeyListener.h"
#include "Components/Text.h"
#include "Components/Shields.h"
#include "Components/ShieldHitAnimator.h"
#include "Components/Inventory.h"
#include "Components/ItemPickup.h"
#include "Components/EconomyAgent.h"
#include "Components/ParallaxTargetAssigner.h"
#include "Components/ShipStatsSink.h"
#include "UI/GameWindow.h"
#include "UI/InventoryWindow.h"
#include "UI/StationWindow.h"
#include "WorldConstants.h"
#include "PlayerData.h"
#include "VectorMath.h"
#include "Economy.h"

void EntityFactory::Init()
{
	// This is where we can setup a component update order because 
	// components are updated in the order of their TypeID's, which
	// are set by calling these functions

	GetComponentTypeID<Position>();
	GetComponentTypeID<Rotation>();
	GetComponentTypeID<Sprite>();
	GetComponentTypeID<AnimatedSprite>();
	GetComponentTypeID<ShieldHitAnimator>();
	GetComponentTypeID<Physics>();
	GetComponentTypeID<SmoothCameraFollow>();
	GetComponentTypeID<ParallaxMovement>();
	GetComponentTypeID<DirectionalKeyboardInput>();
	GetComponentTypeID<ThrusterInput>();
	GetComponentTypeID<ShipThrusters>();
}

EntityID EntityFactory::CreatePlayer(const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(PLAYER_GROUP);

	PlayerData::GetActive()->SetID(ent.GetID());

	MakeIntoPlayer(ent, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateBackground(ResourceID backgroundID, EntityID parallaxTarget)
{
	auto ent = EntityManager::AddEntity(BACKGROUND_GROUP);
	MakeIntoBackground(ent, backgroundID, parallaxTarget);
	return ent.GetID();
}

EntityID EntityFactory::CreateProjectile(const std::string& projId, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(PROJECTILE_GROUP);
	MakeIntoBullet(ent, "LaserOne", sourceEntity, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateShip(const std::string& shipName, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(NON_PLAYER_SHIP_GROUP);
	MakeIntoShip(ent, shipName, p, radians, true);
	return ent.GetID();
}

EntityID EntityFactory::CreateStation(const std::string& stationID, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(STATION_GROUP);
	MakeIntoStation(ent, stationID, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateExplosion(const std::string& explosionID, const b2Vec2& p)
{
	auto ent = EntityManager::AddEntity(EXPLOSION_GROUP);
	ent->AddComponent<Position>(p);
	ent->AddComponent<Rotation>();
	auto& spr = ent->AddComponent<AnimatedSprite>(explosionID);
	ent->AddComponent<Lifetime>(spr.GetAnimation()->GetLength() * spr.GetAnimation()->GetSpeed() * .95f);
	return ent.GetID();
}

EntityID EntityFactory::CreatPickup(const std::string& pickupType, const b2Vec2& p)
{
	auto ent = EntityManager::AddEntity(PICKUP_GROUP);
	ent->AddComponent<Position>(p);
	ent->AddComponent<Rotation>();
	auto& phys = ent->AddComponent<Physics>();
	ent->AddComponent<ItemPickup>();
	auto& spr = ent->AddComponent<Sprite>(pickupType);

	auto dimensions = spr.GetDimensions();
	auto shape = sf::RectangleShape({dimensions.width, dimensions.height});
	shape.setOrigin(B2VecToSFMLVec(spr.GetOrigin()));

	phys.AddShape(shape, .2f, IS_SENSOR, COLLIDES_WITH_SHIP);

	return ent.GetID();
}

EntityID EntityFactory::CreateSpawner(float time, const std::string& shipID, const b2Vec2& pos)
{
	auto ent = EntityManager::AddEntity(BACKGROUND_GROUP);
	ent->AddComponent<Position>(pos);
	ent->AddComponent<ShipSpawner, float, ShipResourceSelector, SpawnLocationSelector>(time, ShipResourceSelector(shipID), SpawnLocationSelector());
	return ent.GetID();
}

EntityID EntityFactory::CreatePlayerSpawner(const b2Vec2& pos)
{
	auto ent = EntityManager::AddEntity(BACKGROUND_GROUP);
	ent->AddComponent<Position>(pos);
	ent->AddComponent<ShipSpawner, EventType, ShipResourceSelector, SpawnLocationSelector, bool>(
		EventType::PlayerDied, 
		ShipResourceSelector([] {return PlayerData::GetActive()->GetPlayerShip(); }), 
		SpawnLocationSelector(), true);
	return ent.GetID();
}

EntityID EntityFactory::CreateMusicPlayer(const std::string& fileName)
{
	auto ent = EntityManager::AddEntity(BACKGROUND_GROUP);
	ent->AddComponent<MusicSource, const std::string&>(fileName);
	return ent->GetID();
}

void EntityFactory::MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p, float radians)
{
	MakeIntoShip(ent, PlayerData::GetActive()->GetPlayerShip(), p, radians, false);
	
	// This makes sure the the players ship stats stay loaded
	ent->AddComponent<ShipStatsSink, std::shared_ptr<ShipStats>>(LoadShip(PlayerData::GetActive()->GetPlayerShip()));

	// player specifiic components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float>(1.5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();
	ent->AddComponent<ZoomHandler>();
	ent->AddComponent<PlayerDeathBroadcaster>();
	ent->AddComponent<SoundListener>();
	ent->AddComponent<GunHeatUIDisplay<DirectionalGun>>();

	auto& shields = ent->GetComponent<Shields>();
	auto& keyListener = ent->AddComponent<KeyListener, 
						std::initializer_list<sf::Keyboard::Key>>({ sf::Keyboard::Num1, sf::Keyboard::Num2, sf::Keyboard::Num3, sf::Keyboard::Num4, sf::Keyboard::I });
	
	keyListener += [&shields](sf::Keyboard::Key k) 
	{ 
		switch (k)
		{
		case sf::Keyboard::Num1:
			shields.SetActive(Shields::Direction::Front);
			break;
		case sf::Keyboard::Num2:
			shields.SetActive(Shields::Direction::Side);
			break;
		case sf::Keyboard::Num3:
			shields.SetActive(Shields::Direction::Rear);
			break;
		case sf::Keyboard::Num4:
			shields.SetActive(Shields::Direction::All);
			break;
		case sf::Keyboard::I:
			auto window = GameWindow::GetWindow<InventoryWindow>("inventory");
			window->Show(true);
			window->SetTarget(PlayerData::GetActive()->GetID());
			break;
		}
	};
}

void EntityFactory::MakeIntoBackground(EntityHandle& ent, ResourceID backgroundID, EntityID parallaxTarget)
{
	ent->AddComponent<Position>();
	ent->AddComponent<TilingBackground, ResourceID>(backgroundID);
	ent->AddComponent<ParallaxMovement, EntityID, float>(parallaxTarget, .1f);
	ent->AddComponent<ParallaxTargetAssigner>();
}

void EntityFactory::MakeIntoBullet(EntityHandle& ent, const std::string& id, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	auto projStats = LoadProjectile(id);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<BulletPhysics, EntityID, const std::string&>(sourceEntity, id);
	ent->AddComponent<CollisionFilterComponent, EntityID>(sourceEntity);
	ent->AddComponent<RectPrimitive, float, float>(projStats->GetSize().x, projStats->GetSize().y);
	ent->AddComponent<Lifetime, float>(projStats->GetLifeTime());
}

void EntityFactory::MakeIntoShip(EntityHandle& ent, const std::string& shipName, const b2Vec2& p, float radians, bool npc)
{
	auto shipStats = LoadShip(shipName);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	auto& sp = ent->AddComponent<Sprite, const std::string&>(shipStats->GetImageLocation());
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<ShipThrusters, ShipThrust*>(shipStats->GetShipThrust());
	auto& shotSound = ent->AddComponent<SoundSource, ResourceID>(shipStats->GetDirGunData()->soundID);
	auto& gun = ent->AddComponent<DirectionalGun, DirectionalGunData*>(shipStats->GetDirGunData());
	gun.SetSoundSource(&shotSound);
	ent->AddComponent<Health>();
	auto& shields = ent->AddComponent<Shields>(shipStats->GetShieldData());
	shields.SetActive(Shields::Direction::All);
	ent->AddComponent<DamageOnAttacked, std::initializer_list<AttackedEventModifier*>>({ &shields });
	ent->AddComponent<ShieldHitAnimator, float>(.75f);
	ent->AddComponent<Inventory>();
	auto& econAgent = ent->AddComponent<EconomyAgent>();

	econAgent.AddItem(Item::Create(ItemType::Credits, 1000));
	econAgent.AddItem(Item::Create(ItemType::Ore, 1000));
	econAgent.AddItem(Item::Create(ItemType::Food, 1000));

	econAgent.SetSellPrices(
	{
		{ ItemType::Ore, Economy::GetBaselinePrice(ItemType::Ore) + 20 },
		{ ItemType::Food, Economy::GetBaselinePrice(ItemType::Food) + 2 }
	});
	econAgent.SetBuyPrices(
	{
		{ ItemType::Ore, Economy::GetBaselinePrice(ItemType::Ore) - 10 },
		{ ItemType::Food, Economy::GetBaselinePrice(ItemType::Food) - 2 }
	});

	if (npc)
	{
		ent->AddComponent<ShipController, std::shared_ptr<ShipStats>>(shipStats);
		// All ships can sense the player
		ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(shipStats->GetSensorRange(), {PLAYER_GROUP});
		ent->AddComponent<ShipAI, std::shared_ptr<ShipStats>>(shipStats);
	}

	auto spriteBox = sp.GetDimensions();
	auto origin = sf::Vector2f(spriteBox.width, spriteBox.height) / 2.f;
	auto& verts = shipStats->GetColliderVertices();
	auto shape = sf::ConvexShape(verts.size());
	for (size_t i = 0; i < verts.size(); i++)
	{
		shape.setPoint(i, (verts[i] * METERS_PER_PIXEL) - origin);
	}

	phys.AddShape(shape, .2f, IS_SHIP, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
	phys.SetPosition(p);

	auto sprites = std::vector<AnimatedSprite*>();
	for (auto tl : shipStats->GetThrusterLocations())
	{
		auto& as = ent->AddComponent<AnimatedSprite, const std::string&>("exhaust-one", OriginOption::MiddleRight);
		as.SetOffset(SFMLVecToB2Vec((tl* METERS_PER_PIXEL) - origin));

		sprites.push_back(&as);
	}

	ent->AddComponent<ThrusterAnimator, const std::vector<AnimatedSprite*>&>(sprites);

	ent->destroyCallback = [](Entity* destoryedEnt)
	{
		auto& pos = destoryedEnt->GetComponent<Position>();
		auto& inventory = destoryedEnt->GetComponent<Inventory>();

		CreateExplosion("explosion-one", pos.position);
		auto pickupID = CreatPickup("crate", pos.position);
		auto pickupHandle = EntityManager::Get(pickupID);
		auto& pickup = pickupHandle->GetComponent<ItemPickup>();

		for (auto& item : inventory)
		{
			pickup.AddItem(item);
		}
	};
}

void EntityFactory::MakeIntoStation(EntityHandle& ent, const std::string& stationID, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);

	auto& sp = ent->AddComponent<Sprite, const std::string&>(stationID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 10.f);
	ent->AddComponent<Inventory>();
	auto& econAgent = ent->AddComponent<EconomyAgent>();
	econAgent.AddItem(Item::Create(ItemType::FuelCells, 1000));

	econAgent.SetSellPrices(
	{
		{ ItemType::Ore, Economy::GetBaselinePrice(ItemType::Ore) + 20 },
		{ ItemType::Food, Economy::GetBaselinePrice(ItemType::Food) + 2 },
		{ ItemType::FuelCells, Economy::GetBaselinePrice(ItemType::FuelCells) }
	});
	econAgent.SetBuyPrices(
	{
		{ ItemType::Ore, Economy::GetBaselinePrice(ItemType::Ore) + 10 },
		{ ItemType::Food, Economy::GetBaselinePrice(ItemType::Food) + 2 }
	});

	auto& sensor = ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(5.f, {PLAYER_GROUP});
	auto& text = ent->AddComponent<Text, const std::string&>("Press E to Interact");
	auto& keyListener = ent->AddComponent<KeyListener, std::initializer_list<sf::Keyboard::Key>>({ sf::Keyboard::E });
	sensor.AttachComponent(&keyListener);
	sensor.AttachComponent(&text);

	EntityID stationEntID = ent.GetID();
	keyListener += [stationEntID](sf::Keyboard::Key)
	{ 
		auto stationWindow = GameWindow::GetWindow<StationWindow>("station_window");
		stationWindow->Show(true);
		stationWindow->SetTarget(stationEntID);
	};

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, 1.f, IS_STATION, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
	phys.SetPosition(p);
}