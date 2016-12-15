// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EconomyID.h>

long long EconomyID::DEFAULT_ID = 0;
long long m_lastID = 0;

EconomyID::EconomyID()
	: ID(DEFAULT_ID), agentType(EconomyAgentType::None)
{
}

EconomyID::EconomyID(long long id, EconomyAgentType agentType)
	: ID(id), agentType(agentType)
{
}

EconomyID EconomyID::Create(EconomyAgentType agentType)
{
	// Find a new ID
	EconomyID id{ m_lastID++, agentType };

	return id;
}

EconomyID EconomyID::GetDefault()
{
	return EconomyID();
}

bool EconomyID::IsDefault() const
{
	return ID == DEFAULT_ID;
}

bool EconomyID::operator==(const EconomyID& other) const
{
	return ID == other.ID;
}

bool EconomyID::operator!=(const EconomyID& other) const
{
	return !(*this == other);
}
