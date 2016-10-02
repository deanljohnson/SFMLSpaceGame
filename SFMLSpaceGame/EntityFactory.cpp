#include <EntityFactory.h>
#include <resource.h>
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Components/RectPrimitive.h>
#include <Components/DirectionalKeyboardInput.h>
#include <Components/DirectionalVelocity.h>
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

void EntityFactory::MakeIntoPlayer(Entity* ent, const b2Vec2& p, float radians) 
{
	MakeIntoShip(ent, SHIP_HUMAN_FIGHTER, p, radians);

	// player specifiic components
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float, float>(.8f, .5f);
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

void EntityFactory::MakeIntoBullet(Entity* ent, const b2Vec2& p, float radians)
{
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	ent->AddComponent<BulletPhysics, b2Vec2, float>(b2Vec2(.3f, .03f), 15.f);
	ent->AddComponent<RectPrimitive>(.3f, .03f);
	ent->AddComponent<Lifetime, float>(5.f);
}

void EntityFactory::MakeIntoShip(Entity* ent, ResourceID shipID, const b2Vec2& p, float radians)
{
	//TODO: load ship stats based on ID
	ent->AddComponent<Position, const b2Vec2&>(p);
	ent->AddComponent<Rotation>(radians);
	auto sp = ent->AddComponent<Sprite, ResourceID>(shipID);
	auto phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<ShipThrusters, const ShipThrust&>(ShipThrust(1.1f, .8f, .5f));
	ent->AddComponent<DirectionalGun, const DirectionalGunData&>(DirectionalGunData(.1f,
	{
		HardPoint(b2Vec2(.5f, -.1f), 0.f),
		HardPoint(b2Vec2(.5f, .1f), 0.f)
	}));

	auto spriteBox = sp.GetPixelLocalBounds();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(p);
}