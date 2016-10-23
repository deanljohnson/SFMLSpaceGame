#pragma once
#include <ShipStats.h>

class ShipStatsSink : public Component
{
private:
	std::shared_ptr<ShipStats> m_shipStats;
public:
	explicit ShipStatsSink(std::shared_ptr<ShipStats> stats)
		: m_shipStats(stats)
	{}

	ShipStats* operator->() { return m_shipStats.get(); };
};