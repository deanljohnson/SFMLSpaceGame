#pragma once
#include <Box2D/Dynamics/b2Body.h>
#include <cereal/access.hpp>

namespace sf
{
	class Shape;
}

class Position;
class Rotation;

class Physics : public Component
{
private:
	Position& m_position;
	Rotation& m_rotation;
	b2Body* m_body{ nullptr };
	b2BodyType m_bodyType;
	float m_linDamping;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_bodyType, m_linDamping);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<Physics>& construct)
	{
		EntityID selfID;
		b2BodyType type;
		float damping;
		ar(selfID, type, damping);
		construct(selfID, type, damping);
	}

public:
	explicit Physics(EntityID ent);
	Physics(EntityID ent, b2BodyType t, float linearDamping);
	~Physics();

	virtual void Update() override;

	void SetPosition(const b2Vec2& v);
	b2Vec2 GetPosition() const;

	void SetVelocity(const b2Vec2& v);
	b2Vec2 GetVelocity() const;

	b2Vec2 GetHeading();

	float GetAngularVelocity() const;

	float GetRotationRadians();

	b2Body* GetBody() { return m_body; }

	void AddShape(const sf::Shape& s, float density, int categoryBits, int collidesWithBits);
};