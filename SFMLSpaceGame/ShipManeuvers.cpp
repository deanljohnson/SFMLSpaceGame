#include "ShipManeuvers.h"
#include "Steering.h"

void ShipManeuvers::Follow(Physics* selfPhysics, 
						   ShipThrusters* selfThrusters, 
						   Physics* target, 
						   float followDistance)
{
	b2Vec2 targetHeading = Steering::Follow(selfPhysics, target, followDistance);

	// Steering towards target
	float targetAngle = atan2f(targetHeading.y, targetHeading.x);
	selfThrusters->SteerTowardsAngle(targetAngle, 1.5f);

	// Moving towards target
	b2Vec2 dif = target->GetPosition() - selfPhysics->GetPosition();
	float amount = std::min(1.f, dif.LengthSquared() / (followDistance * followDistance * 20));
	if (amount > .05f)
		selfThrusters->ApplyThrust(Front, amount);
}

void ShipManeuvers::Intercept(Physics* selfPhysics, 
							  ShipThrusters* selfThrusters, 
							  Physics* target, 
							  float leadModifier)
{
	b2Vec2 targetHeading = Steering::Pursue(selfPhysics, target, leadModifier);

	// Steering towards target
	float targetAngle = atan2f(targetHeading.y, targetHeading.x);
	selfThrusters->SteerTowardsAngle(targetAngle, 1.5f);

	// Moving towards target
	b2Vec2 dif = target->GetPosition() - selfPhysics->GetPosition();
	float amount = std::min(1.f, dif.LengthSquared() / leadModifier);
	if (amount > .05f)
		selfThrusters->ApplyThrust(Front, amount);
}