#pragma once
#include "Box2D/Common/b2Math.h"
#include <vector>
#include "Components/Physics.h"

// Contains methods returning a unit vector that when
// aimed towards will provide a certain behaviour
namespace Steering
{
	// Returns a unit vector towards target
	b2Vec2 Seek(const b2Vec2& current, const b2Vec2& target);

	// Returns a unit vector away from target
	b2Vec2 Flee(const b2Vec2& current, const b2Vec2& target);

	// Returns a unit vector towards a targets future position
	b2Vec2 Pursue(Physics& self, Physics* target, float maxSpeed);

	// Returns a unit vector away from a targets future position
	b2Vec2 Evade(Physics& self, Physics* target, float maxSpeed);

	// Returns a unit vector towards a positiong behind the target
	b2Vec2 Follow(Physics& self, Physics* target, float followDistance);

	// Returns a unit vector that represents the alignment 
	// vector based on the given Physics component's angles
	b2Vec2 AlignOrientation(std::vector<Physics*> others);

	// Returns a unit vector that represents the alignment 
	// vector based on the given Physics component's velocities
	b2Vec2 AlignHeading(std::vector<Physics*> others);
}