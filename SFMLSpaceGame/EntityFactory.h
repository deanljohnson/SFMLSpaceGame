#pragma once
#include <ResourceLoader.h>
#include <EntityHandle.h>

struct StationRecord;
struct ShipRecord;
struct AsteroidRecord;
struct SpriteKey;

namespace EntityFactory
{
	void Init();

	EntityID CreatePlayer(const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateBackground(ResourceID backgroundID, EntityID parallaxTarget);
	EntityID CreateProjectile(std::shared_ptr<ProjectileStats> proj, EntityID sourceEntity, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateMissile(std::shared_ptr<MissileStats> missile, EntityID sourceEntity, EntityID target, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateShip(const std::string& shipID, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateShip(const ShipRecord& record);
	EntityID CreateStation(const std::string& stationID, const std::string& stationName, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
	EntityID CreateStation(const StationRecord& record);
	EntityID CreateExplosion(const std::string& explosionID, const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreatPickup(const std::string& crateType, const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreateAsteroid(const SpriteKey& sprite, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	EntityID CreateAsteroid(const AsteroidRecord& record);
	EntityID CreateSpawner(float time, const std::string& shipID, const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreatePlayerSpawner(const b2Vec2& p = b2Vec2(0, 0));
	EntityID CreateMusicPlayer(const std::string& fileName);

	void MakeIntoPlayer(EntityHandle& ent, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoBackground(EntityHandle& ent, ResourceID backgroundID, EntityID parallaxTarget);
	void MakeIntoMissile(EntityHandle& ent, std::shared_ptr<MissileStats> missile, EntityID sourceEntity, EntityID target, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoAsteroid(EntityHandle& ent, const SpriteKey& key, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f);
	void MakeIntoShip(EntityHandle& ent, const std::string& shipName, const b2Vec2& p = b2Vec2(0.f, 0.f), float radians = 0.f, bool npc = true);
	void MakeIntoStation(EntityHandle& ent, const std::string& stationID, const b2Vec2& p = b2Vec2(0, 0), float radians = 0);
}
