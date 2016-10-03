#pragma once
#include <Components/Position.h>
#include <Components/Rotation.h>
#include <Box2D/Dynamics/b2Body.h>

class BulletPhysics : public Component
{
private:
	Position* m_position{ nullptr };
	Rotation* m_rotation{ nullptr };
	b2Body* m_body{ nullptr };
	b2Vec2 m_size;
	float m_speed;

	//returns whether or not a collision happened
	bool HandleCollisions();

public:
	explicit BulletPhysics(b2Vec2 size, float speed) 
		: m_size(size), m_speed(speed)
	{}
	~BulletPhysics();

	virtual void Init() override;
	virtual void Update() override;
};