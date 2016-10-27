#include "stdafx.h"
#include <EntityHelpers.h>
#include "Components/Position.h"

Entity* EntityHelpers::GetClosestEntity(Entity* subject, const std::vector<Entity*>& source, std::function<bool(Entity*)> filter)
{
	// Reject invalid base cases
	if (subject == nullptr
		|| source.empty()
		|| !subject->HasComponent<Position>())
		return nullptr;

	Position* subjPos = &subject->GetComponent<Position>();

	float closestSquareDistance = std::numeric_limits<float>::max();
	Entity* closestEntity = nullptr;
	for (auto e : source)
	{
		// If an entity doesn't have a position, we are done with it
		if ((filter != nullptr && !filter(e)) || !e->HasComponent<Position>())
			continue;

		// Get the distance between the two positions
		Position* pos = &e->GetComponent<Position>();
		float thisDistanceSquared = (*subjPos - *pos).LengthSquared();

		// Check for a new closest
		if (thisDistanceSquared < closestSquareDistance)
		{
			closestSquareDistance = thisDistanceSquared;
			closestEntity = e;
		}
	}

	return closestEntity;
}