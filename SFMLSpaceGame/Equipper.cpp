#include "stdafx.h"
#include <Equipper.h>
#include <Item.h>
#include <EntityManager.h>
#include <Components/ShipStatsComponent.h>

namespace Equipper
{
	namespace
	{
		void DoLaserRigEquip(LaserRigItem* laserRig, EntityID id, size_t slot)
		{
			auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			shipStats->GetDirGunData()->LoadNewLaserRig(laserRig->rigName, slot);
		}

		size_t GetNumLaserRigSlots(EntityID id)
		{
			auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			return shipStats->GetDirGunData()->rigs.size();
		}
	}

	void Equip(Item* item, EntityID id, size_t slot)
	{
		switch(item->type)
		{
		case ItemType::LaserRig: 
			DoLaserRigEquip(static_cast<LaserRigItem*>(item), id, slot); 
			break;
		default: break;
		}
	}

	size_t GetNumSlots(ItemType type, EntityID id)
	{
		switch (type)
		{
		case ItemType::LaserRig:
			return GetNumLaserRigSlots(id);
		}
		return 0;
	}
}