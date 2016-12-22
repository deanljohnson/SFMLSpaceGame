// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <FindPurchaseJobResult.h>

FindPurchaseJobResult::FindPurchaseJobResult()
{
}

FindPurchaseJobResult::FindPurchaseJobResult(ItemType type, const std::string& detail, EconomyAgent* agent)
	: type(type),
	detail(detail),
	agent(agent)
{
}