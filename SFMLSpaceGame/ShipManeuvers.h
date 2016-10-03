#pragma once
#include "Components/Physics.h"
#include "Components/ShipThrusters.h"

namespace ShipManeuvers
{
	// Uses the given selfPhysics and selfThrusters pointers to follow the target
	void Follow(Physics* selfPhysics, ShipThrusters* selfThrusters, Physics* target, float followDistance);
	
	// Uses the given selfPhysics and selfThrusters pointers to intercept the target
	void Intercept(Physics* selfPhysics, ShipThrusters* selfThrusters, Physics* target, float leadModifier);

	// Uses the given selfPhysics and selfThrusters pointers to intercept the target
	void Approach(Physics* selfPhysics, ShipThrusters* selfThrusters, Physics* target, float distance);
}