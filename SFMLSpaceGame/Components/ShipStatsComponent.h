#pragma once
#include <ShipStats.h>

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
	ShipStatsComponent(EntityID ent, const std::string& shipID)
		: Component(ent),
		  m_shipID(shipID),
		  m_shipStats(LoadShip(m_shipID))
	{}

	ShipStats* operator->() { return m_shipStats.get(); };
};