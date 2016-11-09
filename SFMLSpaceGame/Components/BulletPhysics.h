#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Box2D/Dynamics/b2Body.h>
#include <ProjectileStats.h>

class BulletPhysics : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	b2Body* m_body{ nullptr };
	EntityID m_sourceEntity;

	std::shared_ptr<ProjectileStats> m_projStats;

	//returns whether or not a collision happened
	bool HandleCollisions();

public:
	explicit BulletPhysics(EntityID ent, EntityID sourceEnt, const std::string& projID);
	~BulletPhysics();

	virtual void Update() override;
};