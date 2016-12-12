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
	SpriteKey imageLocation; // The location of this stations sprite
	bool hasShipEditor; // Does this station allow ship editing?
	bool hasRigEditor; // Does this station allow rig editing?
	bool hasChangeShip; // Does this station allow players to change ships?
	bool hasTrade; // Does this station have a trader?

	StationStats();

	static std::string GetTypeName();
};