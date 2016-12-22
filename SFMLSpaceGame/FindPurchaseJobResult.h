#pragma once

enum class ItemType;
class EconomyAgent;

struct FindPurchaseJobResult
{
	FindPurchaseJobResult();
	FindPurchaseJobResult(ItemType type, const std::string& detail, EconomyAgent* agent);
	ItemType type;
	std::string detail;
	EconomyAgent* agent;
};