#pragma once
#include <EntityFactory.h>
#include <StationRecord.h>
#include <ShipRecord.h>
#include <AsteroidRecord.h>

class WorldDefinition 
{
private:
	enum class ObjectType 
	{
		Station, Ship, Asteroid
	};

	static std::unordered_map<std::string, ObjectType> StringToObjectType;
	static std::unordered_map<ObjectType, std::string> ObjectTypeToString;
	friend class cereal::access;

	template<class Archive>
	void load(Archive& ar) 
	{
		while (true)
		{
			const auto namePtr = ar.getNodeName();

			if (!namePtr)
				break;

			ObjectType type = StringToObjectType[namePtr];
			switch (type)
			{
			case ObjectType::Station: {
				StationRecord record;
				ar(record);
				EntityFactory::CreateStation(record);
				break;
				}
			case ObjectType::Ship: {
				ShipRecord record;
				ar(record);
				EntityFactory::CreateShip(record);
				break;
				}
			case ObjectType::Asteroid: {
				AsteroidRecord record;
				ar(record);
				EntityFactory::CreateAsteroid(record);
				break;
				}
			}
		}
	}
public:
	static std::string GetTypeName();
};