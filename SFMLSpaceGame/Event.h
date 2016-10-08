#pragma once

#include <EntityID.h>

namespace sf{
	class Event;
}

enum EventType
{
	None, Attacked
};

struct Event
{
	struct AttackedEvent
	{
		EntityID attackerID;
		float damage;
	};
	
	union
	{
		AttackedEvent attacked;
	};

	EventType type;
};