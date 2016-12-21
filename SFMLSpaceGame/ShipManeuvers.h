#pragma once
#include <Components/Physics.h>
#include <Components/ShipThrusters.h>

namespace ShipManeuvers
{
	// Uses the given selfPhysics and selfThrusters pointers to follow the target
	void Follow(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target, float followDistance);
	
	// Uses the given selfPhysics and selfThrusters pointers to intercept the target
	void Intercept(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target, float leadModifier);

	// Uses the given selfPhysics and selfThrusters pointers to intercept the target
	void Approach(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target, float distance);

	// Uses the given selfPhysics and selfThrusters pointers to face the target
	void FaceTarget(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target);

	// Uses the given selfPhysics and selfThrusters pointers to face the target while compensating for the targets speed
	void FaceTargetForAttack(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target, float projectileSpeed, float steerMultiplier = 1.f);

	// Uses the given selfPhysics and selfthrusters to strafe Left or Right while maintaining the given distance
	void StrafeAtDistanceForAttack(Physics& selfPhysics, ShipThrusters& selfThrusters, Physics* target, ThrustDirection dir, float projectileSpeed, float strafeDistance);
}