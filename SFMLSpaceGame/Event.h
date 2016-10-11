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
	// a unique ID for this event. 
	// A copy of an event will have the same ID
	long id = m_nextID++;
};