#include "stdafx.h"
#include <ContactFilter.h>
#include "Entity.h"
#include "Components/CollisionFilterComponent.h"

bool ContactFilter::ShouldCollide(b2Fixture* a, b2Fixture* b)
{
	auto aBody = a->GetBody();
	auto bBody = b->GetBody();

	// entities will always collide with bodies that do not contain user-data
	if (aBody->GetUserData() == nullptr || bBody->GetUserData() == nullptr)
		return b2ContactFilter::ShouldCollide(a, b);

	Entity* aEntity = static_cast<Entity*>(aBody->GetUserData());
	Entity* bEntity = static_cast<Entity*>(bBody->GetUserData());

	bool shouldCollide = true;
	// filter according to a's CollisionFilterComponent
	if (aEntity->HasComponent<CollisionFilterComponent>())
	{
		CollisionFilterComponent& cf = aEntity->GetComponent<CollisionFilterComponent>();
		//assert(cf.entity != nullptr);
		shouldCollide = cf.ShouldCollide(bEntity) ? shouldCollide : false;
	}

	// filter according to b's CollisionFilterComponent
	if (bEntity->HasComponent<CollisionFilterComponent>())
	{
		CollisionFilterComponent& cf = bEntity->GetComponent<CollisionFilterComponent>();
		//assert(cf.entity != nullptr);
		shouldCollide = cf.ShouldCollide(aEntity) ? shouldCollide : false;
	}

	if (!shouldCollide) 
		return shouldCollide;

	return b2ContactFilter::ShouldCollide(a, b);
}

