#pragma once
#include <cereal\cereal.hpp>
#include <Serializer.h>

class MissileStats 
{
	SERIALIZED_WITH_NAME("missile")
private:
	float m_thrust;
	float m_damage;
	std::string m_imageLocation;
public:
	MissileStats() 
		: m_thrust(0),
		  m_damage(0),
		  m_imageLocation()
	{}

	MissileStats(float thrust, float damage, const std::string& imageLocation)
		: m_thrust(thrust),
		  m_damage(damage),
		  m_imageLocation(imageLocation)
	{}

	inline float GetThrust() const { return m_thrust; }
	inline float GetDamage() const { return m_damage; }
	inline std::string GetImageLocation() const { return m_imageLocation; }

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Thrust", m_thrust),
				cereal::make_nvp("Damage", m_damage),
				cereal::make_nvp("ImageLocation", m_imageLocation));
	}
};