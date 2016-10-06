#include <EntityFactory.h>
#include <resource.h>
#include <EntityGroups.h>
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

void EntityFactory::MakeIntoPlayer(Entity* ent, const b2Vec2& p, float radians) 
{
	MakeIntoShip(ent, SHIP_HUMAN_FIGHTER, p, radians, false);

	// player specifiic components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float>(1.5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();
}

void EntityFactory::MakeIntoBackgroundOne(Entity* ent, Entity* parallaxTarget) 
{
	ent->AddComponent<Position>();
	ent->AddComponent<Background, ResourceID>(BGONE_FRONT);
	ent->AddComponent<ParallaxMovement, Entity&, float>(*parallaxTarget, .1f);
}

void EntityFactory::MakeIntoBullet(Entity* ent, ResourceID id, Entity* sourceEntity, const b2Vec2& p, float radians)
{
	auto projStats = LoadProjectile(id);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<BulletPhysics, const std::shared_ptr<ProjectileStats>&>(projStats);
	ent->AddComponent<CollisionFilterComponent, Entity*>(sourceEntity);
	ent->AddComponent<RectPrimitive, float, float>(projStats->GetSize().x, projStats->GetSize().y);
	ent->AddComponent<Lifetime, float>(projStats->GetLifeTime());
}

void EntityFactory::MakeIntoShip(Entity* ent, ResourceID shipID, const b2Vec2& p, float radians, bool npc)
{
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

void EntityFactory::MakeIntoStation(Entity* ent, ResourceID stationID, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);

	auto& sp = ent->AddComponent<Sprite, ResourceID>(stationID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 10.f);

	ent->AddComponent<EntitySensor, float, Group>(5.f, PLAYER_GROUP);

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(p);
}