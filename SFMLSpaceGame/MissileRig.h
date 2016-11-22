#pragma once
#include <Rig.h>
#include <Serializer.h>
#include <cereal/types/base_class.hpp>

class MissileRig : public Rig
{
	SERIALIZED_WITH_NAME("MissileRig");

private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Rig>(this),
			CEREAL_NVP(fireRate));
	}
public:
	float fireRate{ 0.f };
};