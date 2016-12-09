#pragma once
#include <Rig.h>

class MiningRig : public Rig
{
private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Rig>(this),
			CEREAL_NVP(miningSpeed),
			CEREAL_NVP(miningAmount),
			CEREAL_NVP(imageLocation));
	}
public:
	float miningSpeed{ 0.f };
	size_t miningAmount{ 0 };
	std::string imageLocation;

	static std::string GetTypeName() { static const std::string name = "MiningRig"; return name; };
};