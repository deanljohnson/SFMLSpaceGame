#pragma once
#include <ShipStats.h>

class ShipStatsSink : public Component
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
	static void load_and_construct(Archive& ar, cereal::construct<ShipStatsSink>& construct)
	{
		EntityID selfID;
		std::string shipID;
		ar(selfID, shipID);
		construct(selfID, shipID);
	}
public:
	ShipStatsSink(EntityID ent, const std::string& shipID)
		: Component(ent),
		  m_shipID(shipID),
		  m_shipStats(LoadShip(m_shipID))
	{}

	ShipStats* operator->() { return m_shipStats.get(); };
};