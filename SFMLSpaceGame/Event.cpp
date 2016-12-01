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
