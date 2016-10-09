#pragma once

#include <EntityID.h>

enum EventType
{
	None, Attacked, PlayerDied
};

struct Event
{
private:
	long m_nextID = 0;
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
	long id = m_nextID++; // a unique ID for this event
};