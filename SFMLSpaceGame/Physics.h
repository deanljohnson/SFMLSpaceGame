#pragma once
#include "Position.h"
#include "Rotation.h"
#include <SFML/Graphics/Shape.hpp>
#include <Box2D/Dynamics/b2Body.h>

class Physics : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
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

	void RotateTowards(const b2Vec2& pos, float torqueScale, float smoothingScale);
	void AddShape(const sf::Shape& s, float density = 1.f);
};