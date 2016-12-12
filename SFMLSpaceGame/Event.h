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
	explicit Event(EventType _type);
	EventType type;
	bool handled;
};

class AttackedEvent : public Event
{
public:
	AttackedEvent(EntityID _attackedID, float _damage, float _collisionX, float _collisionY);

	EntityID attackerID;
	float damage;
	float collisionX;
	float collisionY;
};

class PlayerDiedEvent : public Event
{
public:
	PlayerDiedEvent();
};

class PlayerSpawnedEvent : public Event
{
public:
	explicit PlayerSpawnedEvent(EntityID id);

	// The new ID the player has been assigned to
	EntityID ID;
};

class ItemPickupEvent : public Event
{
public:
	explicit ItemPickupEvent(std::shared_ptr<Item> _item);

	std::shared_ptr<Item> item;
};

// simplifies syntax
template<bool Test, typename A, typename B>
using Case = typename std::conditional<Test, A, B>::type;

// Essentially a compile time switch statement
template<EventType T>
struct EventTypeType_
{
	using type = Case<T == EventType::ItemPickup, ItemPickupEvent,
		Case<T == EventType::PlayerDied, PlayerDiedEvent,
		Case<T == EventType::PlayerSpawned, PlayerSpawnedEvent,
		Case<T == EventType::Attacked, AttackedEvent,
		void>>>>;
};

// simplifies syntax
template<EventType T>
using EventTypeType = typename EventTypeType_<T>::type;
