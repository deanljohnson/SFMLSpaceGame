#pragma once

class Entity;

// Encapsulates additional initialization steps to be
// applied when an entity is loaded. These steps are
// intended to be used for initialiation that depends
// on multiple components but does not warrant the 
// creation of a new component.
namespace EntityInitializer 
{
	enum Type
	{ 
		PlayerKeyListenerSetup, 
		SetThrusterLocations,
		ShipDestroyedCallback,
		AssignDamageModifiers,
		AssignShipStatsData,
		StationInteractListenerSetup,
		StationSpriteBoundsColliderSetup,
		PickupSpriteBoundsColliderSetup,
		MissileSpriteBoundsColliderSetup,
		SetActivePlayerID,

		Count
	};

	// Executes the given intialization operation
	// for the given entity
	void Execute(Type initType, Entity& ent);
	void Execute(const std::vector<Type>& initTypes, Entity& ent);
}