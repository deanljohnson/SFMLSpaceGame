#pragma once
#include <Box2D/Dynamics/b2Body.h>

class MissileStats;
class Physics;
class Rotation;

class MissilePhysics : public Component, public Updateable
{
private:
	Position& m_position;
	Rotation& m_rotation;
	b2Body* m_body{ nullptr };
	EntityID m_sourceEntity;

	float m_thrust;
	float m_damage;

	//returns whether or not a collision happened
	bool HandleCollisions();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_sourceEntity, m_thrust, m_damage);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MissilePhysics>& construct)
	{
		EntityID selfID;
		EntityID sourceID;
		float thrust;
		float damage;
		ar(selfID, sourceID, thrust, damage);
		construct(selfID, sourceID, thrust, damage);
	}

	MissilePhysics(EntityID ent, EntityID sourceEnt, float thrust, float damage);
public:
	MissilePhysics(EntityID ent, EntityID sourceEnt, std::shared_ptr<MissileStats> missile);
	~MissilePhysics();

	virtual void Update() override;

	void SetPosition(const b2Vec2& v);
	b2Vec2 GetPosition() const;

	b2Vec2 GetHeading();
	float GetRotationRadians();

	float GetAngularVelocity() const;

	b2Body* GetBody();

	void AddShape(const sf::Shape& s, float density, int categoryBits, int collidesWithBits);
};