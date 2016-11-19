#pragma once
#include "Entity.h"

namespace EntityHelpers
{
	Entity* GetClosestEntity(Entity* subject, 
							const std::vector<Entity*>& source, 
							std::function<bool(Entity*)> filter = nullptr);

	Entity* GetClosestEntity(const b2Vec2& subject,
							const std::vector<Entity*>& source,
							std::function<bool(Entity*)> filter = nullptr);
}