#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Box2D/Dynamics/b2Body.h>

class MissileStats;

class MissilePhysics : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	b2Body* m_body{ nullptr };
	EntityID m_sourceEntity;

	std::shared_ptr<MissileStats> m_missStats;
	std::string m_projID;

	//returns whether or not a collision happened
	bool HandleCollisions();

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_sourceEntity, m_projID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<MissilePhysics>& construct)
	{
		EntityID selfID;
		EntityID sourceID;
		std::string projID;
		ar(selfID, sourceID, projID);
		construct(selfID, sourceID, projID);
	}

public:
	MissilePhysics(EntityID ent, EntityID sourceEnt, const std::string& projID);
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