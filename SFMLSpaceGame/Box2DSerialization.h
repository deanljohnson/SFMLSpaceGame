#pragma once
#include <cereal\cereal.hpp>
#include <Box2D\Common\b2Math.h>

// Must be placed in cereal namespace in order to be found by cereal
namespace cereal {
	template<class Archive>
	void serialize(Archive& archive, b2Vec2& v) 
	{
		archive(v.x, v.y);
	}
}