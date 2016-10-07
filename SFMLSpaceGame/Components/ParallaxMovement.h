#pragma once
#include <Components/Position.h>
#include <EntityHandle.h>

class ParallaxMovement : public Component
{
private:
	Position* m_position{ nullptr };
	Position* m_targetPosition;
	float m_movementScale;

	b2Vec2 m_targetLastPosition;

public:
	explicit ParallaxMovement(std::shared_ptr<EntityHandle> target, float movementScale)
		: m_movementScale(movementScale)
	{
		auto t = *target.get();
		m_targetPosition = &t->GetComponent<Position>();
	}

	virtual void Init() override;
	virtual void Update() override;
};