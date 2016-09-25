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
		: m_bodyType(b2_dynamicBody),
		  velocity(sf::Vector2f(0.f, 0.f)) 
	{}

	explicit Physics(b2BodyType t) 
		: m_bodyType(t),
		  velocity(sf::Vector2f(0.f, 0.f)) 
	{}

	sf::Vector2f velocity;

	virtual void Init() override;
	virtual void Update() override;
};