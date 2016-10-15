#pragma once
#include "ResourceLoader.h"

class ShipResourceSelector
{
private:
	enum { Single } m_type;
	std::string m_singleID;
public:
	explicit ShipResourceSelector(const std::string& name)
		: m_type(Single), m_singleID(name)
	{}
	std::string Select();
};