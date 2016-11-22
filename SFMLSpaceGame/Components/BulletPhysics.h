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

	float m_speed;
	float m_damage;
	b2Vec2 m_size;

	//returns whether or not a collision happened
	bool HandleCollisions();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_sourceEntity, m_speed, m_damage, m_size);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<BulletPhysics>& construct)
	{
		EntityID selfID;
		EntityID sourceID;
		float speed;
		float damage;
		b2Vec2 size;
		ar(selfID, sourceID, speed, damage, size);
		construct(selfID, sourceID, speed, damage, size);
	}

	BulletPhysics(EntityID ent, EntityID sourceEnt, float speed, float damage, const b2Vec2& size);
public:
	BulletPhysics(EntityID ent, EntityID sourceEnt, std::shared_ptr<ProjectileStats> proj);
	~BulletPhysics();

	virtual void Update() override;
};