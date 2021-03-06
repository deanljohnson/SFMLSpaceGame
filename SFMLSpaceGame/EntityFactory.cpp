// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EntityManager.h>
#include <Entity.h>
#include <EntityGroups.h>
#include <EntityFactory.h>
#include <EntityHandle.h>
#include <Components\Components.h>
#include <PlayerData.h>
#include <MissileStats.h>
#include <StationStats.h>
#include <ProjectileFactory.h>
#include <ItemFactory.h>
#include <SpriteKey.h>

#include <StationRecord.h>
#include <ShipRecord.h>
#include <AsteroidRecord.h>

void EntityFactory::Init()
{
	// Calling these sets the order in which components
	// are updated. Also, it keeps things consistent 
	// when it comes to entity serialization
	GetComponentTypeID<Physics>();
	GetComponentTypeID<Position>();
	GetComponentTypeID<Rotation>();
	GetComponentTypeID<Sprite>();
	GetComponentTypeID<Animator>();
	GetComponentTypeID<Shields>();
	GetComponentTypeID<ShieldHitAnimator>();
	
	GetComponentTypeID<SmoothCameraFollow>();
	GetComponentTypeID<ParallaxMovement>();
	GetComponentTypeID<DirectionalKeyboardInput>();
	GetComponentTypeID<ShipThrusters>();
	GetComponentTypeID<ThrusterInput>();

	GetComponentTypeID<RectPrimitive>();
	GetComponentTypeID<RotateToFaceMouse>();
	GetComponentTypeID<GameWorldClickListener>();
	GetComponentTypeID<SoundSource>();
	GetComponentTypeID<SoundListener>();
	GetComponentTypeID<MusicSource>();
	GetComponentTypeID<DirectionalGun>();
	GetComponentTypeID<MissileLauncher>();
	GetComponentTypeID<MiningLaser>();
	GetComponentTypeID<FireGunOnClick<DirectionalGun>>();
	GetComponentTypeID<FireGunOnClick<MissileLauncher>>();
	GetComponentTypeID<FireGunOnClick<MiningLaser>>();
	GetComponentTypeID<BulletPhysics>();
	GetComponentTypeID<MissilePhysics>();
	GetComponentTypeID<Lifetime>();
	GetComponentTypeID<CollisionFilterComponent>();
	GetComponentTypeID<ShipController>();
	GetComponentTypeID<EntitySensor>();
	GetComponentTypeID<ZoomHandler>();
	GetComponentTypeID<ShipAI>();
	GetComponentTypeID<Health>();
	GetComponentTypeID<DamageOnAttacked>();
	GetComponentTypeID<PlayerDeathBroadcaster>();
	GetComponentTypeID<ThrusterAnimator>();
	GetComponentTypeID<GunHeatUIDisplay<DirectionalGun>>();
	GetComponentTypeID<TilingBackground>();
	GetComponentTypeID<KeyListener>();
	GetComponentTypeID<Text>();
	GetComponentTypeID<Inventory>();
	GetComponentTypeID<ItemPickup>();
	GetComponentTypeID<EconomyAgent>();
	GetComponentTypeID<ParallaxTargetAssigner>();
	GetComponentTypeID<ShipStatsComponent>();
	GetComponentTypeID<StationStatsComponent>();
	GetComponentTypeID<OreVein>();
}

EntityID EntityFactory::CreatePlayer(const b2Vec2& p, float radians)
{
	if (false)
	{
		Serializer<> s;
		auto ent = s.Load<Entity>("player");
		PlayerData::GetActive()->SetID(ent->GetID());
		return ent->GetID();
	}
	else
	{
		auto ent = EntityManager::AddEntity(PLAYER_GROUP);

		PlayerData::GetActive()->SetID(ent.GetID());

		MakeIntoPlayer(ent, p, radians);

		Serializer<> s;
		s.Save<Entity>(ent.GetRawPointer(), "player", "player");

		return ent.GetID();
	}
}

EntityID EntityFactory::CreateBackground(ResourceID backgroundID, EntityID parallaxTarget)
{
	auto ent = EntityManager::AddEntity(BACKGROUND_GROUP);
	MakeIntoBackground(ent, backgroundID, parallaxTarget);
	return ent.GetID();
}

EntityID EntityFactory::CreateProjectile(std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	return ProjectileFactory::CreateProjectile(proj, sourceEntity, p, radians);
}

EntityID EntityFactory::CreateMissile(std::shared_ptr<MissileStats> missile, EntityID sourceEntity, EntityID target, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(PROJECTILE_GROUP);
	MakeIntoMissile(ent, missile, sourceEntity, target, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateShip(const std::string& shipID, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(NON_PLAYER_SHIP_GROUP);
	MakeIntoShip(ent, shipID, p, radians, true);
	return ent.GetID();
}

EntityID EntityFactory::CreateShip(const ShipRecord& record)
{
	return CreateShip(record.ID, record.pos, record.angle);
}

EntityID EntityFactory::CreateStation(const std::string& stationID, const std::string& stationName, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(STATION_GROUP, stationName);
	MakeIntoStation(ent, stationID, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateStation(const StationRecord& record) 
{
	return CreateStation(record.ID, record.name, record.pos, record.angle);
}

EntityID EntityFactory::CreateExplosion(const std::string& explosionID, const b2Vec2& p)
{
	auto ent = EntityManager::AddEntity(EXPLOSION_GROUP);
	ent->AddComponent<Position>(p);
	ent->AddComponent<Rotation>();
	auto& spr = ent->AddComponent<Sprite, const std::string&>(explosionID);
	auto& animator = ent->AddComponent<Animator, const std::string&, int>(explosionID, ent->GetComponentID(spr));
	ent->AddComponent<Lifetime>(animator.GetLength() * animator.GetSpeed());
	return ent.GetID();
}

EntityID EntityFactory::CreatPickup(const std::string& pickupType, const b2Vec2& p)
{
	auto ent = EntityManager::AddEntity(PICKUP_GROUP);
	ent->AddComponent<Position>(p);
	ent->AddComponent<Rotation>();
	ent->AddComponent<Physics>();
	ent->AddComponent<ItemPickup>();
	ent->AddComponent<Sprite>(pickupType);

	ent->ApplyInitializer(EntityInitializer::Type::PickupSpriteBoundsColliderSetup);

	return ent.GetID();
}

EntityID EntityFactory::CreateAsteroid(const SpriteKey& sprite, const b2Vec2& p, float radians)
{
	auto ent = EntityManager::AddEntity(ASTEROID_GROUP);
	MakeIntoAsteroid(ent, sprite, p, radians);
	return ent.GetID();
}

EntityID EntityFactory::CreateAsteroid(const AsteroidRecord& record)
{
	return CreateAsteroid(record.sprite, record.pos, record.angle);
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
	return ent.GetID();
}

void EntityFactory::MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p, float radians)
{
	MakeIntoShip(ent, PlayerData::GetActive()->GetPlayerShip(), p, radians, false);

	// player specific components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float>(1.5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick<DirectionalGun>, sf::Mouse::Button>(sf::Mouse::Left);
	ent->AddComponent<FireGunOnClick<MissileLauncher>, sf::Mouse::Button>(sf::Mouse::Right);
	ent->AddComponent<FireGunOnClick<MiningLaser>, sf::Mouse::Button>(sf::Mouse::Middle);
	ent->AddComponent<ZoomHandler>();
	ent->AddComponent<PlayerDeathBroadcaster>();
	ent->AddComponent<SoundListener>();
	ent->AddComponent<GunHeatUIDisplay<DirectionalGun>>();

	ent->GetComponent<Shields>();
	ent->AddComponent<KeyListener, 
		std::initializer_list<sf::Keyboard::Key>>(
		{ 
			sf::Keyboard::Num1, 
			sf::Keyboard::Num2, 
			sf::Keyboard::Num3, 
			sf::Keyboard::Num4, 
			sf::Keyboard::I 
		});
	
	ent->ApplyInitializer(EntityInitializer::Type::PlayerKeyListenerSetup);
	ent->ApplyInitializer(EntityInitializer::Type::SetActivePlayerID);
}

void EntityFactory::MakeIntoBackground(EntityHandle& ent, ResourceID backgroundID, EntityID parallaxTarget)
{
	ent->AddComponent<Position>();
	ent->AddComponent<TilingBackground, ResourceID>(backgroundID);
	ent->AddComponent<ParallaxMovement, EntityID, float>(parallaxTarget, .1f);
	ent->AddComponent<ParallaxTargetAssigner>();
}

void EntityFactory::MakeIntoMissile(EntityHandle& ent, std::shared_ptr<MissileStats> missile, EntityID sourceEntity, EntityID target, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<MissilePhysics, EntityID, std::shared_ptr<MissileStats>>(sourceEntity, missile);
	ent->AddComponent<MissileController, EntityID, EntityID, float>(sourceEntity, target, missile->thrust);
	ent->AddComponent<CollisionFilterComponent, EntityID>(sourceEntity);
	ent->AddComponent<Sprite, const SpriteKey&>(missile->imageLocation);
	ent->AddComponent<Lifetime, float>(30.f);

	ent->ApplyInitializer(EntityInitializer::Type::MissileSpriteBoundsColliderSetup);
}

void EntityFactory::MakeIntoAsteroid(EntityHandle& ent, const SpriteKey& key, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<Sprite, const SpriteKey&, OriginOption>(key, OriginOption::Center);
	ent->AddComponent<Physics>();
	auto& ore = ent->AddComponent<OreVein>();
	ore.SetAmount(1000);

	ent->ApplyInitializer(EntityInitializer::Type::AsteroidSpriteBoundsColliderSetup);
}

void EntityFactory::MakeIntoShip(EntityHandle& ent, const std::string& shipName, const b2Vec2& p, float radians, bool npc)
{
	auto& shipStats = ent->AddComponent<ShipStatsComponent, const std::string&>(shipName);
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	ent->AddComponent<Sprite, const SpriteKey&>(shipStats->imageLocation);
	ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<ShipThrusters, const std::string&>(shipName);
	auto& gunSound = ent->AddComponent<SoundSource, ResourceID>(shipStats->GetDirGunData()->soundID);
	ent->AddComponent<DirectionalGun>().SetSoundSource(&gunSound);
	ent->AddComponent<MissileLauncher>();
	ent->AddComponent<MiningLaser>();
	ent->AddComponent<Health>();
	ent->AddComponent<Shields>();
	ent->AddComponent<DamageOnAttacked>();
	ent->AddComponent<ShieldHitAnimator, float>(.75f);
	ent->AddComponent<Inventory>();

	auto& econAgent = ent->AddComponent<EconomyAgent, EconomyAgentType>(EconomyAgentType::MerchantVessel);
	econAgent.AddItem(ItemFactory::Create<ItemType::Credits>(1000));
	econAgent.AddItem(ItemFactory::Create<ItemType::Ore>(1000));
	econAgent.AddItem(ItemFactory::Create<ItemType::Food>(1000));

	if (npc)
	{
		ent->AddComponent<ShipController, const std::string&>(shipName);
		// All ships can sense the player
		ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(shipStats->sensorRange, {PLAYER_GROUP});
		ent->AddComponent<ShipAI, const std::string&>(shipName);
	}

	auto& thrustAnim = ent->AddComponent<ThrusterAnimator>();
	for (auto tl : shipStats->GetThrusterLocations())
	{
		auto& spr = ent->AddComponent<Sprite, const std::string&>("exhaust-one", OriginOption::MiddleRight);
		ent->AddComponent<Animator, const std::string&, int>("exhaust-one", ent->GetComponentID<Sprite>(spr));
		thrustAnim.AddSprite(&spr);
	}

	ent->ApplyInitializer(EntityInitializer::Type::SetThrusterLocations);
	ent->ApplyInitializer(EntityInitializer::Type::ShipDestroyedCallback);
	ent->ApplyInitializer(EntityInitializer::Type::AssignDamageModifiers);
	ent->ApplyInitializer(EntityInitializer::Type::AssignShipStatsData);
}

void EntityFactory::MakeIntoStation(EntityHandle& ent, const std::string& stationID, const b2Vec2& p, float radians)
{
	auto& stationStats = ent->AddComponent<StationStatsComponent, const std::string&>(stationID);
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	ent->AddComponent<Sprite, const SpriteKey&>(stationStats->imageLocation);
	ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 10.f);
	ent->AddComponent<Inventory>();
	ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(5.f, { PLAYER_GROUP });
	ent->AddComponent<Text, const std::string&>("Press E to Interact");
	ent->AddComponent<KeyListener, std::initializer_list<sf::Keyboard::Key>>({ sf::Keyboard::E });

	auto& econAgent = ent->AddComponent<EconomyAgent, EconomyAgentType>(EconomyAgentType::TradeStation);
	econAgent.AddItem(ItemFactory::Create<ItemType::FuelCells>(1200));
	econAgent.AddItem(ItemFactory::Create<ItemType::LaserRig>(4, "Cannon-Two"));
	econAgent.AddItem(ItemFactory::Create<ItemType::MissileRig>(2, "Hammer"));

	ent->ApplyInitializer(EntityInitializer::Type::StationInteractListenerSetup);
	ent->ApplyInitializer(EntityInitializer::Type::StationSpriteBoundsColliderSetup);
}