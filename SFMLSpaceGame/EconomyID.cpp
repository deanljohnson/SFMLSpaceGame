// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <EconomyID.h>
#include <unordered_set>

std::string EconomyID::DEFAULT_ID = "econ_0";
std::unordered_set<EconomyID> m_existingIDs{};

EconomyID::EconomyID()
	: ID(DEFAULT_ID), agentType(EconomyAgentType::None)
{
}

EconomyID::EconomyID(const std::string& id, EconomyAgentType agentType)
	: ID(id), agentType(agentType)
{
}

EconomyID::~EconomyID()
{
	if (m_existingIDs.empty())
		return;

	if (m_existingIDs.find(*this) != m_existingIDs.end())
		m_existingIDs.erase(*this);
}

EconomyID EconomyID::Create(EconomyAgentType agentType)
{
	std::stringstream ss;
	ss << "econ_" << rand();

	// Find a new ID
	EconomyID id{ ss.str(), agentType };
	while (m_existingIDs.find(id) != m_existingIDs.end())
	{
		// reset the stream
		std::stringstream().swap(ss);

		ss << "econ_" << rand();
		id.ID = ss.str();
	}

	m_existingIDs.emplace(id);

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
