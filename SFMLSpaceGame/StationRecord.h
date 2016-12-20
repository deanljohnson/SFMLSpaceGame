#pragma once
#include <Box2D\Common\b2Math.h>

struct StationRecord 
{
	std::string ID;
	std::string name;
	b2Vec2 pos;
	float angle;

	template<class Archive>
	void serialize(Archive& ar) 
	{
		ar(cereal::make_nvp("ID", ID),
			cereal::make_nvp("name", name),
			cereal::make_nvp("pos", pos),
			cereal::make_nvp("ang", angle));
	}
};