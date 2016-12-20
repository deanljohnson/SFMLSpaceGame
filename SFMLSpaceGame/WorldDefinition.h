#pragma once
#include <StationRecord.h>
#include <ShipRecord.h>
#include <AsteroidRecord.h>
#include <EconomyRecord.h>

class WorldDefinition 
{
private:
	enum class ObjectType 
	{
		Station, Ship, Asteroid, Economy
	};

	static std::unordered_map<std::string, ObjectType> StringToObjectType;
	static std::unordered_map<ObjectType, std::string> ObjectTypeToString;

	void HandleLoad(const StationRecord& record);
	void HandleLoad(const ShipRecord& record);
	void HandleLoad(const AsteroidRecord& record);
	void HandleLoad(const EconomyRecord& record);

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
				HandleLoad(record);
				break;
				}
			case ObjectType::Ship: {
				ShipRecord record;
				ar(record);
				HandleLoad(record);
				break;
				}
			case ObjectType::Asteroid: {
				AsteroidRecord record;
				ar(record);
				HandleLoad(record);
				break;
				}
			case ObjectType::Economy: {
				EconomyRecord record;
				ar(record);
				HandleLoad(record);
				}
			}
		}
	}
public:
	static std::string GetTypeName();
};