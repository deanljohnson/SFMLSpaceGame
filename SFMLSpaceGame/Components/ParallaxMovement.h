#pragma once
#include <Components/Position.h>
#include <EntityHandle.h>

class ParallaxMovement : public Component
{
private:
	EntityHandle m_targetHandle;
	Position& m_position;
	Position& m_targetPosition;
	float m_movementScale;

	b2Vec2 m_targetLastPosition;

public:
	ParallaxMovement(EntityID ent, EntityID target, float movementScale);

	virtual void Update() override;

	void SetTarget(const EntityHandle& targetHandle);
	bool HasValidTarget();
};