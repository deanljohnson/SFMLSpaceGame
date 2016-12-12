// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <StationStats.h>

StationStats::StationStats()
	: imageLocation(),
	hasShipEditor(false),
	hasRigEditor(false),
	hasChangeShip(false),
	hasTrade(false)
{
}

std::string StationStats::GetTypeName()
{
	return "Station";
}