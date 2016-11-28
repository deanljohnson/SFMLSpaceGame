#pragma once
#include <EntityHandle.h>
#include <cereal/access.hpp>
#include <Box2DSerialization.h>

class Position;

class ParallaxMovement : public Component
{
private:
	EntityHandle m_targetHandle;
	Position& m_position;
	Position& m_targetPosition;
	float m_movementScale;

	b2Vec2 m_targetLastPosition;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_targetHandle.GetID(), m_movementScale, m_targetLastPosition);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ParallaxMovement>& construct)
	{
		EntityID selfID;
		EntityID targetID;
		float movementScale;
		ar(selfID, targetID, movementScale);
		construct(selfID, targetID, movementScale);

		ar(construct->m_targetLastPosition);
	}

public:
	ParallaxMovement(EntityID ent, EntityID target, float movementScale);

	virtual void Update() override;

	void SetTarget(const EntityHandle& targetHandle);
	bool HasValidTarget();
};