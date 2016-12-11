// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/StationStatsComponent.h>
#include <StationStats.h>

StationStatsComponent::StationStatsComponent(EntityID ent, const std::string& shipID)
	: Component(ent),
	m_stationID(shipID),
	m_stationStats(LoadStation(m_stationID))
{}

StationStats* StationStatsComponent::operator->()
{
	return m_stationStats.get();
}