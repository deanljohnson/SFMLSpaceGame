#pragma once
#include "ResourceLoader.h"

class ShipResourceSelector
{
private:
	enum { Single, Func } m_type;
	
	std::string m_singleID;
	std::function<std::string()> m_sourceFunc;
public:
	explicit ShipResourceSelector(const std::string& name)
		: m_type(Single), m_singleID(name)
	{}
	explicit ShipResourceSelector(std::function<std::string()> func)
		: m_type(Func), m_sourceFunc(func)
	{}

	std::string Select();
};