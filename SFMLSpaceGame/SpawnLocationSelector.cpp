#include "stdafx.h"
#include <SpawnLocationSelector.h>

b2Vec2 SpawnLocationSelector::Select(const b2Vec2& spawnerLocation)
{
	switch (m_type)
	{
	case AtSpawner:
		return spawnerLocation;
	}

	return spawnerLocation;
}
