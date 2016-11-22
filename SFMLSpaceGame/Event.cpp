#include "stdafx.h"
#include <Event.h>

Event::Event()
	: type(EventType::None)
{}

Event::Event(const Event& other)
	: type(other.type),
	  handled(other.handled)
{
	switch(type)
	{
	case EventType::ItemPickup:
		itemPickup = other.itemPickup;
		break;
	case EventType::PlayerSpawned:
		playerSpawned = other.playerSpawned;
		break;
	case EventType::PlayerDied:
		playerDied = other.playerDied;
		break;
	case EventType::Attacked:
		attacked = other.attacked;
		break;
	}
}

Event::~Event()
{
	switch (type)
	{
	case EventType::ItemPickup:
		itemPickup.~ItemPickupEvent();
		break;
	}
}

Event& Event::operator=(const Event& other)
{
	type = other.type;
	handled = other.handled;

	switch(type)
	{
	case EventType::ItemPickup:
		itemPickup = other.itemPickup;
		break;
	case EventType::PlayerSpawned:
		playerSpawned = other.playerSpawned;
		break;
	case EventType::PlayerDied:
		playerDied = other.playerDied;
		break;
	case EventType::Attacked:
		attacked = other.attacked;
		break;
	}

	return *this;
}