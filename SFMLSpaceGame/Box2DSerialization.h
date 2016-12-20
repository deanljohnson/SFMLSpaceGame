#pragma once
#include <cereal\cereal.hpp>
#include <Box2D\Common\b2Math.h>

// Must be placed in cereal namespace in order to be found by cereal
namespace cereal {
	template<class Archive>
	void serialize(Archive& ar, b2Vec2& v) 
	{
		ar(cereal::make_nvp("x", v.x), 
			cereal::make_nvp("y", v.y));
	}
}