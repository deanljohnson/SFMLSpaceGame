#pragma once
#include "ResourceLoader.h"

class ShipResourceSelector
{
private:
	enum { Single } m_type;
	ResourceID m_singleID;
public:
	explicit ShipResourceSelector(ResourceID id)
		: m_type(Single), m_singleID(id)
	{}
	ResourceID Select();
};