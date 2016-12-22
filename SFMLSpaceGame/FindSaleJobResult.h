#pragma once
class EconomyAgent;

struct FindSaleJobResult
{
	explicit FindSaleJobResult(EconomyAgent* agent = nullptr);
	EconomyAgent* agent;
};