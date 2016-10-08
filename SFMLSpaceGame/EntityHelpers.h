#pragma once
#include "Entity.h"

namespace EntityHelpers
{
	Entity* GetClosestEntity(Entity* subject, const std::vector<Entity*>& source);
}