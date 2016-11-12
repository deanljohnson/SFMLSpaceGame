#include "stdafx.h"
#include <EntityInitializer.h>
#include <Entity.h>
#include <EntityFactory.h>
#include <EntityManager.h>

#include <PlayerData.h>
#include <UI\InventoryWindow.h>
#include <UI\StationWindow.h>
#include <Components\Shields.h>
#include <Components\KeyListener.h>
#include <Components\Inventory.h>
#include <Components\ItemPickup.h>
#include <Components\AnimatedSprite.h>
#include <Components\ThrusterAnimator.h>
#include <Components\ShipStatsComponent.h>
#include <Components\DamageOnAttacked.h>
#include <Components\EntitySensor.h>
#include <Components\Text.h>
#include <CollisionGroups.h>
#include <VectorMath.h>

void DoPlayerKeyListenerSetup(Entity& ent) 
{
	auto& shields = ent.GetComponent<Shields>();
	auto& keyListener = ent.GetComponent<KeyListener>();

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

void DoShipDestroyedCallback(Entity& ent) 
{
	ent.destroyCallback = [](Entity* destoryedEnt)
	{
		auto& pos = destoryedEnt->GetComponent<Position>();
		auto& inventory = destoryedEnt->GetComponent<Inventory>();

		EntityFactory::CreateExplosion("explosion-one", pos.position);
		auto pickupID = EntityFactory::CreatPickup("crate", pos.position);
		auto pickupHandle = EntityManager::Get(pickupID);
		auto& pickup = pickupHandle->GetComponent<ItemPickup>();

		for (auto& item : inventory)
		{
			pickup.AddItem(item);
		}
	};
}

void DoAssignThrusterAnimatorSprites(Entity& ent) 
{
	auto& shipStats = ent.GetComponent<ShipStatsComponent>();
	auto& sp = ent.GetComponent<Sprite>();
	auto spriteBox = sp.GetDimensions();
	auto origin = sf::Vector2f(spriteBox.width, spriteBox.height) / 2.f;

	auto& anim = ent.GetComponent<ThrusterAnimator>();
	auto spr = &ent.GetComponent<AnimatedSprite>();

	for (auto tl : shipStats->GetThrusterLocations())
	{
		assert(spr != nullptr);

		spr->SetOffset(SFMLVecToB2Vec((tl* METERS_PER_PIXEL) - origin));

		anim.AddSprite(spr);
		spr = static_cast<AnimatedSprite*>(spr->next);
	}
}

void DoAssignShipStatsData(Entity& ent)
{
	auto& shipStats = ent.GetComponent<ShipStatsComponent>();
	
	auto& dirGun = ent.GetComponent<DirectionalGun>();
	dirGun.SetGunData(shipStats->GetDirGunData());
	// This is kind of out of place, but until we have
	// a way to serialize the connection between components
	// this is the best we can do. The big problem is how
	// do we know that this is the right sound source? If 
	// there are multiple component on the entity, this 
	// will likely break
	auto& soundSource = ent.GetComponent<SoundSource>();
	dirGun.SetSoundSource(&soundSource);

	auto& shields = ent.GetComponent<Shields>();
	shields.SetShieldData(shipStats->GetShieldData());

	// Only npc ships will have an EntitySensor
	if (ent.HasComponent<EntitySensor>())
	{
		auto& entSensor = ent.GetComponent<EntitySensor>();
		entSensor.SetRange(shipStats->GetSensorRange());
	}

	auto& sp = ent.GetComponent<Sprite>();
	auto& phys = ent.GetComponent<Physics>();

	auto spriteBox = sp.GetDimensions();
	auto origin = sf::Vector2f(spriteBox.width, spriteBox.height) / 2.f;
	auto& verts = shipStats->GetColliderVertices();
	auto shape = sf::ConvexShape(verts.size());
	for (size_t i = 0; i < verts.size(); i++)
	{
		shape.setPoint(i, (verts[i] * METERS_PER_PIXEL) - origin);
	}

	phys.AddShape(shape, .2f, IS_SHIP, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
}

void DoAssignDamageModifiers(Entity& ent)
{
	auto& shields = ent.GetComponent<Shields>();
	auto& damOnAttack = ent.GetComponent<DamageOnAttacked>();
	damOnAttack.AddModifier(&shields);
}

void DoStationInteractListenerSetup(Entity& ent)
{
	auto& sensor = ent.GetComponent<EntitySensor>();
	auto& text = ent.GetComponent<Text>();
	auto& keyListener = ent.GetComponent<KeyListener>();
	EntityID stationEntID = ent.GetID();
	keyListener += [stationEntID](sf::Keyboard::Key)
	{
		auto stationWindow = GameWindow::GetWindow<StationWindow>("station_window");
		stationWindow->Show(true);
		stationWindow->SetTarget(stationEntID);
	};

	sensor.AttachComponent(&keyListener);
	sensor.AttachComponent(&text);
}

void DoStationSpriteBoundsColliderSetup(Entity& ent)
{
	auto& sp = ent.GetComponent<Sprite>();
	auto& phys = ent.GetComponent<Physics>();

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(B2VecToSFMLVec(sp.GetOrigin()));
	phys.AddShape(shape, 1.f, IS_STATION, COLLIDES_WITH_SHIP | COLLIDES_WITH_BULLET | COLLIDES_WITH_STATION | COLLIDES_WITH_SENSOR);
}

void DoPickupSpriteBoundsColliderSetup(Entity& ent)
{
	auto& sp = ent.GetComponent<Sprite>();
	auto& phys = ent.GetComponent<Physics>();

	auto spriteBox = sp.GetDimensions();
	auto shape = sf::RectangleShape(sf::Vector2f(spriteBox.width, spriteBox.height));
	shape.setOrigin(B2VecToSFMLVec(sp.GetOrigin()));
	phys.AddShape(shape, .2f, IS_SENSOR, COLLIDES_WITH_SHIP);
}

void EntityInitializer::Execute(EntityInitializer::Type initType, Entity& ent) 
{
	switch (initType) 
	{
	case EntityInitializer::Type::PlayerKeyListenerSetup:
		DoPlayerKeyListenerSetup(ent);
		break;
	case EntityInitializer::Type::ShipDestroyedCallback:
		DoShipDestroyedCallback(ent);
		break;
	case EntityInitializer::Type::AssignThrusterAnimatorSprites:
		DoAssignThrusterAnimatorSprites(ent);
		break;
	case EntityInitializer::Type::AssignShipStatsData:
		DoAssignShipStatsData(ent);
		break;
	case EntityInitializer::Type::AssignDamageModifiers:
		DoAssignDamageModifiers(ent);
		break;
	case EntityInitializer::Type::StationInteractListenerSetup:
		DoStationInteractListenerSetup(ent);
		break;
	case EntityInitializer::Type::StationSpriteBoundsColliderSetup:
		DoStationSpriteBoundsColliderSetup(ent);
		break;
	case EntityInitializer::Type::PickupSpriteBoundsColliderSetup:
		DoPickupSpriteBoundsColliderSetup(ent);
		break;
	}
}

void EntityInitializer::Execute(const std::vector<Type>& initTypes, Entity& ent)
{
	for (auto& it : initTypes)
	{
		Execute(it, ent);
	}
}