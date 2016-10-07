#pragma once
#include <ResourceLoader.h>
#include <EntityHandle.h>

namespace EntityFactory
{
	void MakeIntoPlayer(std::shared_ptr<EntityHandle> ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoBackgroundOne(std::shared_ptr<EntityHandle> ent, std::shared_ptr<EntityHandle> parallaxTarget);
	void MakeIntoBullet(std::shared_ptr<EntityHandle> ent, ResourceID projId, Entity* sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoShip(std::shared_ptr<EntityHandle> ent, ResourceID shipID, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f, bool npc = true);
	void MakeIntoStation(std::shared_ptr<EntityHandle> ent, ResourceID stationID, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
}
