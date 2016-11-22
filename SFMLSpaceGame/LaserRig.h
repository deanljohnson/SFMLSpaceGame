#pragma once
#include <Rig.h>
#include <Serializer.h>
#include <cereal/types/base_class.hpp>
#include <ProjectileStats.h>

class LaserRig : public Rig 
{
	SERIALIZED_WITH_NAME("LaserRig");

private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar) 
	{
		ar(cereal::base_class<Rig>(this), 
			CEREAL_NVP(fireRate), 
			CEREAL_NVP(heatLimit), 
			CEREAL_NVP(cooldownRate), 
			CEREAL_NVP(heatGenerated), 
			cereal::make_nvp("projSpeed", projectile->speed), 
			cereal::make_nvp("projLifetime", projectile->lifeTime), 
			cereal::make_nvp("projDamage", projectile->damage), 
			cereal::make_nvp("projSize", projectile->size));
	}
public:
	float fireRate{0.f};
	float heatLimit{0.f};
	float cooldownRate{0.f};
	float heatGenerated{0.f};
	std::shared_ptr<ProjectileStats> projectile{new ProjectileStats()};
};