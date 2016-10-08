#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Box2D/Dynamics/b2Body.h>
#include <ProjectileStats.h>
#include <EntityHandle.h>

class BulletPhysics : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	b2Body* m_body{ nullptr };
	EntityID m_sourceEntity;

	std::shared_ptr<ProjectileStats> m_projStats;

	//returns whether or not a collision happened
	bool HandleCollisions();

public:
	explicit BulletPhysics(EntityID sourceEnt, const std::shared_ptr<ProjectileStats>& projStats)
		: m_sourceEntity(sourceEnt), m_projStats(projStats)
	{}
	~BulletPhysics();

	virtual void Init() override;
	virtual void Update() override;
};