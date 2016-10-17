#pragma once

#include <EntityID.h>

enum EventType
{
	None, Attacked, PlayerDied
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

	union
	{
		AttackedEvent attacked;
		PlayerDiedEvent playerDied;
	};

	EventType type;
	bool handled = false;
};