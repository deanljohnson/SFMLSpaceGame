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
#include "Components/ShipAI.h"
#include "Components/Health.h"
#include "Components/DamageOnAttacked.h"
#include "CollisionGroups.h"

void EntityFactory::MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p, float radians)
{
	MakeIntoShip(ent, SHIP_HUMAN_FIGHTER, p, radians, false);
	
	// player specifiic components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float>(1.5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();
	ent->AddComponent<ZoomHandler>();
}

void EntityFactory::MakeIntoBackgroundOne(EntityHandle& ent, EntityID parallaxTarget)
{
	ent->AddComponent<Position>();
	ent->AddComponent<Background, ResourceID>(BGONE_FRONT);
	ent->AddComponent<ParallaxMovement, EntityID, float>(parallaxTarget, .1f);
}

void EntityFactory::MakeIntoBullet(EntityHandle& ent, ResourceID id, EntityID sourceEntity, const b2Vec2& p, float radians)
{
	auto projStats = LoadProjectile(id);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation, float>(radians);
	ent->AddComponent<BulletPhysics, EntityID, const std::shared_ptr<ProjectileStats>&>(sourceEntity, projStats);
	ent->AddComponent<CollisionFilterComponent, EntityID>(sourceEntity);
	ent->AddComponent<RectPrimitive, float, float>(projStats->GetSize().x, projStats->GetSize().y);
	ent->AddComponent<Lifetime, float>(projStats->GetLifeTime());
}

void EntityFactory::MakeIntoShip(EntityHandle& ent, ResourceID shipID, const b2Vec2& p, float radians, bool npc)
{
	auto shipStats = LoadShip(shipID);

	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	auto& sp = ent->AddComponent<Sprite, ResourceID>(shipID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<ShipThrusters, ShipThrust*>(shipStats->GetShipThrust());
	ent->AddComponent<DirectionalGun, DirectionalGunData*>(shipStats->GetDirGunData());
	ent->AddComponent<Health>();
	ent->AddComponent<DamageOnAttacked>();

	if (npc)
	{
		ent->AddComponent<ShipController, std::shared_ptr<ShipStats>>(shipStats);
		// All ships can sense the player
		ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(shipStats->GetSensorRange(), {PLAYER_GROUP});
		ent->AddComponent<ShipAI>();
	}

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f, IS_SHIP, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
	phys.SetPosition(p);
}

void EntityFactory::MakeIntoStation(EntityHandle& ent, ResourceID stationID, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);

	auto& sp = ent->AddComponent<Sprite, ResourceID>(stationID);
	auto& phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 10.f);

	ent->AddComponent<EntitySensor, float, std::initializer_list<Group>>(5.f, {PLAYER_GROUP});
	ent->AddComponent<TextOnSensor, const std::string&>("Press E to Interact");

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, 1.f, IS_STATION, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
	phys.SetPosition(p);
}