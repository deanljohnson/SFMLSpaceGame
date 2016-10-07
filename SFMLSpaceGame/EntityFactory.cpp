#include <resource.h>
#include <EntityGroups.h>
#include <EntityFactory.h>
#include <EntityHandle.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/RectPrimitive.h>
#include <Components/DirectionalKeyboardInput.h>
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
#include <Components/BulletPhysics.h>
#include <Components/Lifetime.h>
#include "Components/CollisionFilterComponent.h"
#include "Components/ShipController.h"
#include "Components/EntitySensor.h"
#include "Components/TextOnSensor.h"
#include "Components/ZoomHandler.h"

void EntityFactory::MakeIntoPlayer(std::shared_ptr<EntityHandle> entPtr, const b2Vec2& p, float radians)
{
	auto ent = *entPtr.get();

	MakeIntoShip(entPtr, SHIP_HUMAN_FIGHTER, p, radians, false);
	
	// player specifiic components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float>(1.5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();
	ent->AddComponent<ZoomHandler>();
}

void EntityFactory::MakeIntoBackgroundOne(std::shared_ptr<EntityHandle> entPtr, std::shared_ptr<EntityHandle> parallaxTarget)
{
	auto ent = *entPtr.get();

	ent->AddComponent<Position>();
	ent->AddComponent<Background, ResourceID>(BGONE_FRONT);
	ent->AddComponent<ParallaxMovement, std::shared_ptr<EntityHandle>, float>(parallaxTarget, .1f);
}

void EntityFactory::MakeIntoBullet(std::shared_ptr<EntityHandle> entPtr, ResourceID id, Entity* sourceEntity, const b2Vec2& p, float radians)
{
	auto ent = *entPtr.get();

	auto projStats = LoadProjectile(id);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<BulletPhysics, const std::shared_ptr<ProjectileStats>&>(projStats);
	ent->AddComponent<CollisionFilterComponent, Entity*>(sourceEntity);
	ent->AddComponent<RectPrimitive, float, float>(projStats->GetSize().x, projStats->GetSize().y);
	ent->AddComponent<Lifetime, float>(projStats->GetLifeTime());
}

void EntityFactory::MakeIntoShip(std::shared_ptr<EntityHandle> entPtr, ResourceID shipID, const b2Vec2& p, float radians, bool npc)
{
	auto ent = *entPtr.get();

	auto shipStats = LoadShip(shipID);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	auto& sp = ent->AddComponent<Sprite, ResourceID>(shipID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<ShipThrusters, ShipThrust*>(shipStats->GetShipThrust());
	ent->AddComponent<DirectionalGun, DirectionalGunData*>(shipStats->GetDirGunData());

	if (npc)
	{
		ent->AddComponent<ShipController, std::shared_ptr<ShipStats>>(shipStats);
		// All ships can sense the player
		ent->AddComponent<EntitySensor, float, Group>(shipStats->GetSensorRange(), PLAYER_GROUP);
	}

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(p);
}

void EntityFactory::MakeIntoStation(std::shared_ptr<EntityHandle> entPtr, ResourceID stationID, const b2Vec2& p, float radians)
{
	auto ent = *entPtr.get();

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);

	auto& sp = ent->AddComponent<Sprite, ResourceID>(stationID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 10.f);

	ent->AddComponent<EntitySensor, float, Group>(5.f, PLAYER_GROUP);
	ent->AddComponent<TextOnSensor, const std::string&>("Press E to Interact");

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(p);
}