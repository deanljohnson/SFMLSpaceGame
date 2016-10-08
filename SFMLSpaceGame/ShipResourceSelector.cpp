#include <ShipResourceSelector.h>

ResourceID ShipResourceSelector::Select()
{
	switch(m_type)
	{
	case Single:
		return m_singleID;
	}

	return NullResource;
}
