// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
