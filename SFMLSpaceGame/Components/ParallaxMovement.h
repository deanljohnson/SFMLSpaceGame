#pragma once
#include <Components/Position.h>
#include <Entity.h>

class ParallaxMovement : public Component
{
private:
	Position* m_position{ nullptr };
	Position* m_targetPosition;
	float m_movementScale;

	b2Vec2 m_targetLastPosition;

public:
	explicit ParallaxMovement(Entity& target, float movementScale)
		: m_targetPosition(&target.GetComponent<Position>()),
		  m_movementScale(movementScale)
	{}

	virtual void Init() override;
	virtual void Update() override;
};