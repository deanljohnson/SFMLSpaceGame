#pragma once
#include <UI/StringEntryPopup.h>

class RigNameEntry : public StringEntryPopup
{
public:
	RigNameEntry()
		: StringEntryPopup("rig_name_entry", "Select New Rig Name")
	{}
};