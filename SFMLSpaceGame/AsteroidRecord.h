#pragma once
#include <SpriteKey.h>
#include <Box2D\Common\b2Math.h>

struct AsteroidRecord 
{
	SpriteKey sprite;
	b2Vec2 pos;
	float angle;

	template<class Archive>
	void serialize(Archive& ar) 
	{
		ar(cereal::make_nvp("sprite", sprite),
			cereal::make_nvp("pos", pos),
			cereal::make_nvp("ang", angle));
	}
};
