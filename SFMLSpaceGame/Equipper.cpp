#include "stdafx.h"
#include <Equipper.h>
#include <Item.h>
#include <ItemFactory.h>
#include <EntityManager.h>
#include <Components/ShipStatsComponent.h>

namespace Equipper
{
	namespace
	{
		void DoLaserRigEquip(std::shared_ptr<LaserRigItem> laserRig, EntityID id, size_t slot)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			shipStats->GetDirGunData()->LoadNewLaserRig(laserRig->rigName, slot);
		}

		std::shared_ptr<Item> DoLaserRigUnEquip(EntityID id, size_t slot)
		{
			// Get the item
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			const auto& rig = shipStats->GetDirGunData()->rigs[slot];

			// Copy rig name
			auto item = ItemFactory::Create(ItemType::LaserRig, 1);
			static_cast<LaserRigItem*>(item.get())->rigName = rig->name;
			
			// "remove" the old item
			shipStats->GetDirGunData()->rigs[slot] = nullptr;

			return item;
		}

		size_t GetNumLaserRigSlots(EntityID id)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			return shipStats->GetDirGunData()->rigs.size();
		}
	}

	void Equip(std::shared_ptr<Item> item, EntityID id, size_t slot)
	{
		switch(item->type)
		{
		case ItemType::LaserRig: 
			DoLaserRigEquip(std::dynamic_pointer_cast<LaserRigItem>(item), id, slot);
			break;
		default: break;
		}
	}

	std::shared_ptr<Item> Unequip(ItemType type, EntityID id, size_t slot)
	{
		switch (type)
		{
		case ItemType::LaserRig:
			return DoLaserRigUnEquip(id, slot);
		default: break;
		}

		return nullptr;
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