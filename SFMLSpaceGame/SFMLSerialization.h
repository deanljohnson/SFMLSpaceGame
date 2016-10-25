#pragma once
#include <cereal\cereal.hpp>
#include <SFML/System/Vector2.hpp>

// Must be placed in cereal namespace in order to be found by cereal
namespace cereal {
	template<class Archive>
	void serialize(Archive& archive, sf::Vector2f& v)
	{
		archive(CEREAL_NVP(v.x), CEREAL_NVP(v.y));
	}

	template<class Archive>
	void serialize(Archive& archive, sf::IntRect& v)
	{
		archive(cereal::make_nvp("Left", v.left),
				cereal::make_nvp("Top", v.top),
				cereal::make_nvp("Width", v.width),
				cereal::make_nvp("Height", v.height));
	}
}