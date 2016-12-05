// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/ShipStatsComponent.h>
#include <ShipStats.h>

ShipStatsComponent::ShipStatsComponent(EntityID ent, const std::string& shipID)
	: Component(ent),
	  m_shipID(shipID),
	  m_shipStats(LoadShip(m_shipID))
{}

ShipStats* ShipStatsComponent::operator->()
{ 
	return m_shipStats.get();
}