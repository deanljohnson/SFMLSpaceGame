#pragma once
#include <Entity.h>

namespace EntityFactory
{
	void MakeIntoPlayer(Entity* ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoBackgroundOne(Entity* ent, Entity* parallaxTarget);
	void MakeIntoBullet(Entity* ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
}
