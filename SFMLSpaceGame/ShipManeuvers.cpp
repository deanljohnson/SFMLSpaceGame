// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include "ShipManeuvers.h"
#include "Steering.h"
#include "ExtendedMath.h"

void ShipManeuvers::Follow(Physics& selfPhysics, 
						   ShipThrusters& selfThrusters, 
						   Physics* target, 
						   float followDistance)
{
	b2Vec2 targetHeading = Steering::Follow(selfPhysics, target, followDistance);

	selfThrusters.SteerTowardsHeading(targetHeading, 1.5f);

	// Moving towards target
	b2Vec2 dif = target->GetPosition() - selfPhysics.GetPosition();
	float amount = std::min(1.f, dif.LengthSquared() / (followDistance * followDistance * 20));
	if (amount > .05f)
		selfThrusters.ApplyThrust(Front, amount);
}

void ShipManeuvers::Intercept(Physics& selfPhysics, 
							  ShipThrusters& selfThrusters, 
							  Physics* target, 
							  float leadModifier)
{
	b2Vec2 targetHeading = Steering::Pursue(selfPhysics, target, leadModifier);

	selfThrusters.SteerTowardsHeading(targetHeading, 1.5f);

	// Moving towards target
	b2Vec2 dif = target->GetPosition() - selfPhysics.GetPosition();
	float amount = std::min(1.f, dif.LengthSquared() / leadModifier);
	if (amount > .05f)
		selfThrusters.ApplyThrust(Front, amount);
}

void ShipManeuvers::Approach(Physics& selfPhysics, 
							 ShipThrusters& selfThrusters, 
							 Physics* target, 
							 float distance)
{
	b2Vec2 targetHeading = Steering::Seek(selfPhysics.GetPosition(), target->GetPosition());

	selfThrusters.SteerTowardsHeading(targetHeading, 1.5f);

	b2Vec2 dif = target->GetPosition() - selfPhysics.GetPosition();
	float distToTarget = dif.Length();


	// Still a good distance away
	if (distToTarget > distance * 1.f)
	{
		float toApproachPoint = distToTarget - distance;
		float reverseStrength = selfThrusters.GetStrength(Reverse);
		float currentSpeed = selfPhysics.GetVelocity().Length();
		float timeToApproach = toApproachPoint / currentSpeed;
		float possibleSpeedReduction = reverseStrength * timeToApproach;

		// Need to start slowing down or we will overshoot
		if (possibleSpeedReduction < currentSpeed)
			selfThrusters.ApplyThrust(Reverse, 1.f);
		else
		{
			// Only accelerate forward if we are within 30 degrees of facing the target
			if (b2Dot(selfPhysics.GetHeading(), dif) > COS_30)
				selfThrusters.ApplyThrust(Front, 1.f);
		}
			
	}
	else if (distToTarget < distance * .95f)
	{
		selfThrusters.ApplyThrust(Reverse, .1f);
	}
	else selfThrusters.ApplyThrust(Front, .3f);
}

void ShipManeuvers::FaceTarget(Physics& selfPhysics, 
							   ShipThrusters& selfThrusters, 
							   Physics* target)
{
	b2Vec2 targetHeading = Steering::Seek(selfPhysics.GetPosition(), target->GetPosition());

	selfThrusters.SteerTowardsHeading(targetHeading, 1.5f);
}

void ShipManeuvers::FaceTargetForAttack(Physics& selfPhysics,
										ShipThrusters& selfThrusters,
										Physics* target,
										float projectileSpeed,
										float steerMultiplier)
{
	b2Vec2 targetHeading = Steering::Pursue(selfPhysics, target, projectileSpeed);

	selfThrusters.SteerTowardsHeading(targetHeading, 2.f * steerMultiplier);
}

void ShipManeuvers::StrafeAtDistanceForAttack(Physics& selfPhysics, 
											  ShipThrusters& selfThrusters, 
											  Physics* target, 
											  ThrustDirection dir, 
											  float projectileSpeed, 
											  float strafeDistance)
{
	assert(dir == Left || dir == Right);

	FaceTargetForAttack(selfPhysics, selfThrusters, target, projectileSpeed, 3.f);

	b2Vec2 dif = target->GetPosition() - selfPhysics.GetPosition();
	float squaredDist = dif.LengthSquared();
	float strafeDistSquared = strafeDistance * strafeDistance;

	// Too far away, need to move closer
	if (squaredDist > strafeDistSquared * 1.05f) 
	{
		// if we are within 90 degrees of facing the target
		if (b2Dot(selfPhysics.GetHeading(), dif) > 0)
		{
			selfThrusters.ApplyThrust(Front, std::min(1.f, squaredDist / (strafeDistSquared * 1.3f)));
		}
		
		// Start strafing slightly as we approach
		selfThrusters.ApplyThrust(dir, .2f);
	}
	// Too close, need to back up
	else if (squaredDist < strafeDistSquared * .95f) 
	{
		selfThrusters.ApplyThrust(Reverse, .5f);
		// continue strafing, even if we are close
		selfThrusters.ApplyThrust(dir, 1.f);
	}
	else 
	{
		selfThrusters.ApplyThrust(dir, 1.f);
	}
}
