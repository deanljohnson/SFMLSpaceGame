// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EntityHelpers.h>
#include <Entity.h>
#include <Components/Position.h>

Entity* EntityHelpers::GetClosestEntity(Entity* subject, 
										const std::vector<Entity*>& source, 
										std::function<bool(Entity*)> filter)
{
	// Reject invalid base cases
	if (subject == nullptr
		|| source.empty()
		|| !subject->HasComponent<Position>())
		return nullptr;

	auto& subjPos = subject->GetComponent<Position>();

	return GetClosestEntity(subjPos.position, 
							source, 
							[filter, subject](Entity* e) 
							{ 
								return subject->GetID() != e->GetID() && (filter == nullptr || filter(e));
							});
}

Entity* EntityHelpers::GetClosestEntity(const b2Vec2& subject, 
										const std::vector<Entity*>& source, 
										std::function<bool(Entity*)> filter)
{
	float closestSquareDistance = std::numeric_limits<float>::max();
	Entity* closestEntity = nullptr;
	for (auto e : source)
	{
		// If an entity doesn't have a position, we are done with it
		if ((filter != nullptr && !filter(e)) || !e->HasComponent<Position>())
			continue;

		// Get the distance between the two positions
		auto& pos = e->GetComponent<Position>();
		float thisDistanceSquared = (subject - pos.position).LengthSquared();

		// Check for a new closest
		if (thisDistanceSquared < closestSquareDistance)
		{
			closestSquareDistance = thisDistanceSquared;
			closestEntity = e;
		}
	}

	return closestEntity;
}