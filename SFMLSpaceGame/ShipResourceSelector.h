#pragma once

class ShipResourceSelector
{
private:
	enum { Single, Func } m_type;
	
	std::string m_singleID;
	std::function<std::string()> m_sourceFunc;
public:
	explicit ShipResourceSelector(const std::string& name);
	explicit ShipResourceSelector(std::function<std::string()> func);

	std::string Select();
};