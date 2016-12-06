#pragma once
#include <Rig.h>

class MiningRig : public Rig
{
private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<Rig>(this));
	}
public:
	static std::string GetTypeName() { static const std::string name = "MiningRig"; return name; };
};