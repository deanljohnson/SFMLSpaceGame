#pragma once
#include <Box2d\Common\b2Math.h>
#include <cereal\cereal.hpp>

class MissileStats 
{
private:
	float m_thrust;
	float m_damage;
	b2Vec2 m_size;
public:
	MissileStats() 
		: m_thrust(0),
		  m_damage(0),
		  m_size(0, 0)
	{}

	MissileStats(float thrust, float damage, b2Vec2 size)
		: m_thrust(thrust),
		  m_damage(damage),
		  m_size(size) 
	{}

	inline float GetThrust() const { return m_thrust; }
	inline float GetDamage() const { return m_damage; }
	inline b2Vec2 GetSize() const { return m_size; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Thrust", m_thrust),
				cereal::make_nvp("Damage", m_damage),
				cereal::make_nvp("Size", m_size));
	}

	static std::string GetTypeName() { return "missile"; }
};