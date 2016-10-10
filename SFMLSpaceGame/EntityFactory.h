#pragma once
#include <ResourceLoader.h>
#include <EntityHandle.h>

namespace EntityFactory
{
	void Init();

	EntityID CreatePlayer(const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateBackground(ResourceID backgroundID, EntityID parallaxTarget);
	EntityID CreateProjectile(ResourceID projId, EntityID sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateShip(ResourceID shipID, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateStation(ResourceID stationID, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
	EntityID CreateSpawner(float time, ResourceID shipID, const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreatePlayerSpawner(const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreateMusicPlayer(const std::string& fileName);

	void MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoBackground(EntityHandle& ent, ResourceID backgroundID, EntityID parallaxTarget);
	void MakeIntoBullet(EntityHandle& ent, ResourceID projId, EntityID sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoShip(EntityHandle& ent, ResourceID shipID, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f, bool npc = true);
	void MakeIntoStation(EntityHandle& ent, ResourceID stationID, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
}
