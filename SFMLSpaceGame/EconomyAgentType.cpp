// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EconomyAgentType.h>

std::unordered_map<EconomyAgentType, std::string> EconomyAgentTypeToString{
	std::make_pair(EconomyAgentType::None, "None"),
	std::make_pair(EconomyAgentType::TradeStation, "TradeStation"),
	std::make_pair(EconomyAgentType::MerchantVessel, "MerchantVessel")
};

std::unordered_map<std::string, EconomyAgentType> StringToEconomyAgentType{
	std::make_pair("None", EconomyAgentType::None),
	std::make_pair("TradeStation", EconomyAgentType::TradeStation),
	std::make_pair("MerchantVessel", EconomyAgentType::MerchantVessel)
};