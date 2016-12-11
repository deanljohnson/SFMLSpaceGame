#pragma once
class StationStats;

class StationStatsComponent : public Component
{
private:
	std::string m_stationID;
	std::shared_ptr<StationStats> m_stationStats;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_stationID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<StationStatsComponent>& construct)
	{
		EntityID selfID;
		std::string shipID;
		ar(selfID, shipID);
		construct(selfID, shipID);
	}
public:
	StationStatsComponent(EntityID ent, const std::string& shipID);

	StationStats* operator->();;
};