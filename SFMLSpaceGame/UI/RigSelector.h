#pragma once
#include <UI/DataFileSelector.h>
#include <LaserRig.h>

class RigSelector : public DataFileSelector<LaserRig, MissileRig>
{
public:
	RigSelector()
		: DataFileSelector<LaserRig, MissileRig>("rig_select", "Select Rig")
	{ }
};