// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
