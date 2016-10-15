#include <ShipResourceSelector.h>

std::string ShipResourceSelector::Select()
{
	switch(m_type)
	{
	case Single:
		return m_singleID;
	}

	return "";
}
