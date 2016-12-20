// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <WorldDefinition.h>

std::unordered_map<std::string, WorldDefinition::ObjectType> WorldDefinition::StringToObjectType
{
	std::make_pair("Station", ObjectType::Station),
	std::make_pair("Ship", ObjectType::Ship),
	std::make_pair("Asteroid", ObjectType::Asteroid)
};

std::unordered_map<WorldDefinition::ObjectType, std::string> WorldDefinition::ObjectTypeToString
{
	std::make_pair(ObjectType::Station, "Station"),
	std::make_pair(ObjectType::Ship, "Ship"),
	std::make_pair(ObjectType::Asteroid, "Asteroid")
};

std::string WorldDefinition::GetTypeName() 
{
	return "World";
}