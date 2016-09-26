#pragma once
#include "Component.h"
#include "Position.h"
#include <Box2D/Dynamics/b2Body.h>

class Physics : public Component
{
private:
	Position* m_position{ nullptr };
	b2Body* m_body{ nullptr };
	b2BodyType m_bodyType;

public:
	Physics() 
		: m_bodyType(b2_dynamicBody)
	{}

	explicit Physics(b2BodyType t) 
		: m_bodyType(t)
	{}

	virtual void Init() override;
	virtual void Update() override;

	void SetPosition(const b2Vec2& v);
	const b2Vec2 GetPosition() const;

	void SetVelocity(const b2Vec2& v);
	const b2Vec2 GetVelocity() const;
};