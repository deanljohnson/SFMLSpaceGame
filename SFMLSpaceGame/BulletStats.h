#pragma once
#include <Box2d/Common/b2Math.h>

class BulletStats 
{
private:
	float m_speed;
	float m_lifeTime;
	b2Vec2 m_size;
public:
	BulletStats(float speed, float lifeTime, b2Vec2 size)
		: m_speed(speed),
		  m_lifeTime(lifeTime),
		  m_size(size)
	{}

	inline float GetSpeed() const { return m_speed; }
	inline float GetLifeTime() const { return m_lifeTime; }
	inline b2Vec2 GetSize() const { return m_size; }
};