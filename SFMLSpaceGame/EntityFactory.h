#pragma once
#include <Entity.h>

namespace EntityFactory
{
	void MakeIntoPlayer(Entity* ent);
	void MakeIntoBackgroundOne(Entity* ent, Entity* parallaxTarget);
	void MakeIntoBullet(Entity* ent);
}
