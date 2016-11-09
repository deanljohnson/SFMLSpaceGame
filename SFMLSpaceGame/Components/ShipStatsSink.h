#pragma once
#include <ShipStats.h>

class ShipStatsSink : public Component
{
private:
	std::shared_ptr<ShipStats> m_shipStats;
public:
	ShipStatsSink(EntityID ent, std::shared_ptr<ShipStats> stats)
		: Component(ent),
		  m_shipStats(stats)
	{}

	ShipStats* operator->() { return m_shipStats.get(); };
};