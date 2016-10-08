#pragma once
#include <ResourceLoader.h>
#include <EntityHandle.h>

namespace EntityFactory
{
	void MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoBackgroundOne(EntityHandle& ent, EntityHandle& parallaxTarget);
	void MakeIntoBullet(EntityHandle& ent, ResourceID projId, Entity* sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoShip(EntityHandle& ent, ResourceID shipID, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f, bool npc = true);
	void MakeIntoStation(EntityHandle& ent, ResourceID stationID, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
}
