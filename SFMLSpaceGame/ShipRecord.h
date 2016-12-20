#pragma once
#include <string>
#include <Box2D\Common\b2Math.h>

struct ShipRecord
{
	std::string ID;
	b2Vec2 pos;
	float angle;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("ID", ID),
			cereal::make_nvp("pos", pos),
			cereal::make_nvp("ang", angle));
	}
};