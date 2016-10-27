#include "stdafx.h"
#include <ShipResourceSelector.h>

std::string ShipResourceSelector::Select()
{
	switch(m_type)
	{
	case Single:
		return m_singleID;
	case Func:
		return m_sourceFunc();
	}

	return "";
}
