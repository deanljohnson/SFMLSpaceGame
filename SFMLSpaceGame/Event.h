#pragma once

#include <EntityID.h>

class Item;

enum class EventType
{
	None, Attacked, PlayerDied, PlayerSpawned, ItemPickup
};

struct Event
{
public:
	Event();
	Event(const Event& other);
	~Event();

	struct ItemPickupEvent
	{
		explicit ItemPickupEvent(std::shared_ptr<Item> i);
		std::shared_ptr<Item> item;
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

	Event& operator=(const Event& other);
};