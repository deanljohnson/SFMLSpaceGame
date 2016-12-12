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
		ar(CEREAL_NVP(imageLocation),
			CEREAL_NVP(hasShipEditor),
			CEREAL_NVP(hasRigEditor),
			CEREAL_NVP(hasChangeShip),
			CEREAL_NVP(hasTrade));
	}
public:
	SpriteKey imageLocation;
	bool hasShipEditor;
	bool hasRigEditor;
	bool hasChangeShip;
	bool hasTrade;

	StationStats();

	static std::string GetTypeName();
};