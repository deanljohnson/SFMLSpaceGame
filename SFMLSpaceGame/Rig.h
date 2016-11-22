#pragma once
#include <cereal\access.hpp>

class Rig 
{
private:
	friend class cereal::access;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(CEREAL_NVP(name));
	}
public:
	std::string name;
};