#pragma once

#include <EntityID.h>
#include <Item.h>

enum class EventType
{
	None, Attacked, PlayerDied, PlayerSpawned, ItemPickup
};

struct Event
{
public:
	Event()
		: type(EventType::None)
	{};

	struct ItemPickupEvent
	{
		explicit ItemPickupEvent(const Item& i)
			: item(i)
		{}
		Item item;
	};

	struct AttackedEvent
	{
		EntityID attackerID;
		float damage;
		float collisionX;
		float collisionY;
	};

	struct PlayerDiedEvent
	{
		
	};

	struct PlayerSpawnedEvent
	{
		EntityID ID;
	};

	union
	{
		AttackedEvent attacked;
		PlayerDiedEvent playerDied;
		PlayerSpawnedEvent playerSpawned;
		ItemPickupEvent itemPickup;
	};

	EventType type;
	bool handled = false;
};