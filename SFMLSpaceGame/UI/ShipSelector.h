#pragma once
#include <UI/DataFileSelector.h>

class ShipSelector : public DataFileSelector<ShipStats>
{
public:
	ShipSelector()
		: DataFileSelector<ShipStats>("ship_select", "Select Ship")
	{ }
};