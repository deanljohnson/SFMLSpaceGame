#pragma once
#include <cereal/cereal.hpp>
#include <SFML/System/Vector2.hpp>

// Must be placed in cereal namespace in order to be found by cereal
namespace cereal {
	template<class Archive>
	void serialize(Archive& ar, sf::Vector2f& v)
	{
		ar(cereal::make_nvp("x", v.x), 
			cereal::make_nvp("y", v.y));
	}

	template<class Archive>
	void serialize(Archive& ar, sf::IntRect& v)
	{
		ar(cereal::make_nvp("Left", v.left),
			cereal::make_nvp("Top", v.top),
			cereal::make_nvp("Width", v.width),
			cereal::make_nvp("Height", v.height));
	}
}