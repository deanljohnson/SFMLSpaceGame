#include "ShipManeuvers.h"
#include "Steering.h"

void ShipManeuvers::Follow(Physics* selfPhysics, 
						   ShipThrusters* selfThrusters, 
						   Physics* target, 
						   float followDistance)
{
	b2Vec2 targetHeading = Steering::Follow(selfPhysics, target, followDistance);

	selfThrusters->SteerTowardsHeading(targetHeading, 1.5f);

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

	selfThrusters->SteerTowardsHeading(targetHeading, 1.5f);

	// Moving towards target
	b2Vec2 dif = target->GetPosition() - selfPhysics->GetPosition();
	float amount = std::min(1.f, dif.LengthSquared() / leadModifier);
	if (amount > .05f)
		selfThrusters->ApplyThrust(Front, amount);
}

void ShipManeuvers::Approach(Physics* selfPhysics, 
							 ShipThrusters* selfThrusters, 
							 Physics* target, 
							 float distance)
{
	b2Vec2 targetHeading = Steering::Seek(selfPhysics->GetPosition(), target->GetPosition());

	selfThrusters->SteerTowardsHeading(targetHeading, 1.5f);

	b2Vec2 dif = target->GetPosition() - selfPhysics->GetPosition();
	float distToTarget = dif.Length();

	// We are too close and moving towards target
	if (distToTarget < distance * .95f)
	{
		selfThrusters->ApplyThrust(Reverse, .1f);
	}
	// We are close enough and need to slow down to 0
	else if (distToTarget < distance * 1.1f)
	{
		// We are moving slow enough we will let damping slow us down
		if (selfPhysics->GetVelocity().LengthSquared() < .2f)
			return;

		// Moving fast and towards target
		if (b2Dot(dif, selfPhysics->GetVelocity()) > 0)
			selfThrusters->ApplyThrust(Reverse, .1f);
		// Moving fast and away from target
		else
			selfThrusters->ApplyThrust(Front, .1f);
	}
	// We are far enough away to approach regularly
	else
	{
		// Apply thrust that gets weaker within (distance * 1.5f)
		selfThrusters->ApplyThrust(Front, std::min(1.f, distToTarget / (distance * 1.5f)));
	}
}