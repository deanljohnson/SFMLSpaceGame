#pragma once

#include <EntityID.h>

enum EventType
{
	None, Attacked, PlayerDied, PlayerSpawned
};

struct Event
{
public:
	struct AttackedEvent
	{
		EntityID attackerID;
		float damage;
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
	};

	EventType type;
	bool handled = false;
};