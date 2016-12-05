#pragma once
class ShipStats;

class ShipStatsComponent : public Component
{
private:
	std::string m_shipID;
	std::shared_ptr<ShipStats> m_shipStats;

	friend class cereal::access;

	// used for saving
	template <class Archive>
	void serialize(Archive& ar)
	{
		ar(entity.GetID(), m_shipID);
	}

	template <class Archive>
	static void load_and_construct(Archive& ar, cereal::construct<ShipStatsComponent>& construct)
	{
		EntityID selfID;
		std::string shipID;
		ar(selfID, shipID);
		construct(selfID, shipID);
	}
public:
	ShipStatsComponent(EntityID ent, const std::string& shipID);

	ShipStats* operator->();;
};