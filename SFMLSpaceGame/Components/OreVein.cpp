// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/OreVein.h>

OreVein::OreVein(EntityID ent)
	: Component(ent)
{
}

size_t OreVein::Mine(size_t goalAmount)
{
	if (m_ore.amount >= goalAmount)
	{
		m_ore.amount -= goalAmount;
		return goalAmount;
	}
	
	auto minedAmount = m_ore.amount;
	m_ore.amount = 0;
	return minedAmount;
}

void OreVein::SetAmount(size_t amt)
{
	m_ore.amount = amt;
}