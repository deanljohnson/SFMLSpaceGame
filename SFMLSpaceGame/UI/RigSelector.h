#pragma once
#include <UI/DataFileSelector.h>
#include <LaserRig.h>

class RigSelector : public DataFileSelector<LaserRig>
{
public:
	RigSelector()
		: DataFileSelector<LaserRig>("rig_select", "Select Rig")
	{ }
};