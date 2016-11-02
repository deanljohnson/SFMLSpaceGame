#include "stdafx.h"
#include <EconomyID.h>
#include <unordered_set>

std::string EconomyID::DEFAULT_ID = "econ_0";
std::unordered_set<EconomyID> m_existingIDs{};

EconomyID::EconomyID()
	: m_id(DEFAULT_ID)
{
}

EconomyID::EconomyID(const std::string& id)
	: m_id(id)
{
}

EconomyID::~EconomyID()
{
	if (m_existingIDs.empty())
		return;

	if (m_existingIDs.find(*this) != m_existingIDs.end())
		m_existingIDs.erase(*this);
}

EconomyID EconomyID::Create()
{
	std::stringstream ss;
	ss << "econ_" << rand();

	// Find a new ID
	EconomyID id{ ss.str() };
	while (m_existingIDs.find(id) != m_existingIDs.end())
	{
		// reset the stream
		std::stringstream().swap(ss);

		ss << "econ_" << rand();
		id = EconomyID(ss.str());
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
	return m_id == DEFAULT_ID;
}

bool EconomyID::operator==(const EconomyID& other) const
{
	return m_id == other.m_id;
}

bool EconomyID::operator!=(const EconomyID& other) const
{
	return !(*this == other);
}

std::string EconomyID::GetStringID() const
{
	return m_id;
}