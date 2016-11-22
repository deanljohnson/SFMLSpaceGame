#pragma once
#include <Rig.h>
#include <Serializer.h>
#include <cereal/types/base_class.hpp>
#include <MissileStats.h>

class MissileRig : public Rig
{
	SERIALIZED_WITH_NAME("MissileRig");

private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Rig>(this),
			CEREAL_NVP(fireRate),
			cereal::make_nvp("missileThrust", missile->thrust),
			cereal::make_nvp("missileDamage", missile->damage),
			cereal::make_nvp("missileImage", missile->imageLocation));
	}
public:
	float fireRate{ 0.f };
	std::shared_ptr<MissileStats> missile{ new MissileStats() };
};