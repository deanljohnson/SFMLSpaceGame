#include "stdafx.h"
#include <EntityInitializer.h>
#include <Entity.h>
#include <EntityFactory.h>
#include <EntityManager.h>

#include <PlayerData.h>
#include <UI\InventoryWindow.h>
#include <Components\Shields.h>
#include <Components\KeyListener.h>
#include <Components\Inventory.h>
#include <Components\ItemPickup.h>
#include <Components\AnimatedSprite.h>
#include <Components\ThrusterAnimator.h>

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
	auto& anim = ent.GetComponent<ThrusterAnimator>();
	auto spr = &ent.GetComponent<AnimatedSprite>();
	while (spr != nullptr) 
	{
		anim.AddSprite(spr);
		spr = static_cast<AnimatedSprite*>(spr->next);
	}

	
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
		DoShipDestroyedCallback(ent);
		break;
	}
}