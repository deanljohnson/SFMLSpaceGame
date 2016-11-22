#pragma once
#include <Box2D.h>

// Defines the position and angle of
// a ships weapon
struct HardPoint
{
	HardPoint()
		: positionOffset(),
		  angleOffset()
	{}
	HardPoint(b2Vec2 offset, float angle)
		: positionOffset(offset),
		  angleOffset(angle)
	{}

	//Offset from the ships center to the HardPoint location. 
	//+x = towards the ships nose 
	//+y = towards the ships right side
	b2Vec2 positionOffset;
	float angleOffset;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(cereal::make_nvp("Position", positionOffset),
			cereal::make_nvp("Angle", angleOffset));
	}
};