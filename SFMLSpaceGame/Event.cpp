// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Event.h>

Event::Event(EventType _type)
	: type(_type), handled(false)
{
}

AttackedEvent::AttackedEvent(EntityID _attackedID, float _damage, float _collisionX, float _collisionY)
	: Event(EventType::Attacked),
	  attackerID(_attackedID),
	  damage(_damage),
	  collisionX(_collisionX),
	  collisionY(_collisionY)
{
}

PlayerDiedEvent::PlayerDiedEvent()
	: Event(EventType::PlayerDied)
{
}

PlayerSpawnedEvent::PlayerSpawnedEvent(EntityID id)
	: Event(EventType::PlayerSpawned),
	  ID(id)
{
}

ItemPickupEvent::ItemPickupEvent(std::shared_ptr<Item> _item)
	: Event(EventType::ItemPickup),
	  item(_item)
{
}
