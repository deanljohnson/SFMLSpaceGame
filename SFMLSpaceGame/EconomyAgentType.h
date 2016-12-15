#pragma once
#include <unordered_map>

enum class EconomyAgentType : int
{
	None = 0x0, 
	TradeStation = 0x1, 
	MerchantVessel = 0x2
};

extern std::unordered_map<EconomyAgentType, std::string> EconomyAgentTypeToString;
extern std::unordered_map<std::string, EconomyAgentType> StringToEconomyAgentType;