#pragma once
#include <Box2d\Common\b2Math.h>
#include <cereal\cereal.hpp>
#include <Serializer.h>

class ProjectileStats
{
	SERIALIZED_WITH_NAME("projectile")
private:
	float m_speed;
	float m_lifeTime;
	float m_damage;
	b2Vec2 m_size;
public:
	ProjectileStats()
		: m_speed(0),
		  m_lifeTime(0),
		  m_damage(0),
		  m_size(0, 0)
	{}
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

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Speed", m_speed), 
				cereal::make_nvp("Lifetime", m_lifeTime), 
				cereal::make_nvp("Damage", m_damage), 
				cereal::make_nvp("Size", m_size));
	}
};