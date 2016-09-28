#pragma once
#include "Position.h"
#include "Entity.h"

class ParallaxMovement : public Component
{
private:
	Position* m_position{ nullptr };
	Position* m_targetPosition;
	float m_movementScale;

	sf::Vector2f m_targetLastPosition;
	sf::Vector2f m_accumulatedOffset;

public:
	explicit ParallaxMovement(Entity& target, float movementScale)
		: m_targetPosition(&target.GetComponent<Position>()),
		  m_movementScale(movementScale)
	{}

	virtual void Init() override;
	virtual void Update() override;
};