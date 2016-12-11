#pragma once
#include <cereal/access.hpp>
#include <SpriteKey.h>

class StationStats
{
private:
	friend class cereal::access;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(imageLocation));
	}
public:
	SpriteKey imageLocation;

	StationStats();

	static std::string GetTypeName();
};