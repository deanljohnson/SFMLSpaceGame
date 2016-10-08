#pragma once
#include <Components/Position.h>
#include <EntityHandle.h>

class ParallaxMovement : public Component
{
public:
	ParallaxMovement(const ParallaxMovement& other) = delete;
	ParallaxMovement(ParallaxMovement&& other) = delete;
	ParallaxMovement& operator=(const ParallaxMovement& other) = delete;
	ParallaxMovement& operator=(ParallaxMovement&& other) = delete;
private:
	Position* m_position{ nullptr };
	Position* m_targetPosition{ nullptr };
	EntityID m_targetID;
	EntityHandle m_targetHandle;
	float m_movementScale;

	b2Vec2 m_targetLastPosition;

public:
	explicit ParallaxMovement(EntityID target, float movementScale)
		: m_targetID(target),
		  m_movementScale(movementScale)
	{
	}

	virtual void Init() override;
	virtual void Update() override;
	void SetTarget(EntityHandle& targetHandle);
};