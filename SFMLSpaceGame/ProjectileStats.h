#pragma once
#include <Box2d/Common/b2Math.h>

class ProjectileStats 
{
private:
	float m_speed;
	float m_lifeTime;
	float m_damage;
	b2Vec2 m_size;
public:
	ProjectileStats(float speed, float lifeTime, float damage, b2Vec2 size)
		: m_speed(speed),
		  m_lifeTime(lifeTime),
		  m_damage(damage),
		  m_size(size)
	{}

	inline float GetSpeed() const { return m_speed; }
	inline float GetLifeTime() const { return m_lifeTime; }
	inline float GetDamage() const { return m_damage; }
	inline b2Vec2 GetSize() const { return m_size; }
};