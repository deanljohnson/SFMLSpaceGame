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

void EntityFactory::MakeIntoPlayer(Entity* ent) 
{
	auto pos = ent->AddComponent<Position>();
	ent->AddComponent<Rotation>();
	auto sp = ent->AddComponent<Sprite, ResourceID>(SHIP_HUMAN_FIGHTER);
	auto phys = ent->AddComponent<Physics, b2BodyType, float>(b2_dynamicBody, 1.f);
	ent->AddComponent<DirectionalKeyboardInput>();
	ent->AddComponent<ShipThrusters, ShipThrust, ShipThrust>(ShipThrust(.08f, .06f, .04f), ShipThrust(7.f, 5.f, 5.f));
	ent->AddComponent<ThrusterInput>();
	ent->AddComponent<RotateToFaceMouse, float, float>(.8f, .5f);
	ent->AddComponent<SmoothCameraFollow>();
	ent->AddComponent<DirectionalGun>();
	ent->AddComponent<GameWorldClickListener>();
	ent->AddComponent<FireGunOnClick>();

	auto spriteBox = sp.GetPixelLocalBounds();
	sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(shape.getSize() / 2.f);
	phys.AddShape(shape, .2f);
	phys.SetPosition(b2Vec2(5, 5));
}

void EntityFactory::MakeIntoBackgroundOne(Entity* ent, Entity* parallaxTarget) 
{
	ent->AddComponent<Position>();
	ent->AddComponent<Background, ResourceID>(BGONE_FRONT);
	ent->AddComponent<ParallaxMovement, Entity&, float>(*parallaxTarget, .1f);
}