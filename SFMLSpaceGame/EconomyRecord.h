#pragma once
#include <vector>

struct EconomyRecord
{
	std::vector<std::pair<std::string, std::string>> tradeConnections;

	template<class Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::make_nvp("tradeConnections", tradeConnections));
	}
};