#pragma once
#include <UI/StringEntryPopup.h>

class ShipNameEntry : public StringEntryPopup
{
public:
	ShipNameEntry()
		: StringEntryPopup("ship_name_entry", "Select New Ship Name")
	{}
};