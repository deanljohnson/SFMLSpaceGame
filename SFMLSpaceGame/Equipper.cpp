// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Equipper.h>
#include <Item.h>
#include <ItemFactory.h>
#include <EntityManager.h>
#include <Components/ShipStatsComponent.h>
#include "Entity.h"

namespace Equipper
{
	namespace
	{
		void DoLaserRigEquip(const LaserRigItem& laserRig, EntityID id, size_t slot)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			shipStats->GetDirGunData()->LoadNewLaserRig(laserRig.rigName, slot);
		}

		Item DoLaserRigUnEquip(EntityID id, size_t slot)
		{
			// Get the item
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			const auto& rig = shipStats->GetDirGunData()->rigs[slot];

			// Copy rig name
			auto item = ItemFactory::Create<ItemType::LaserRig>(1);
			item.rigName = rig->name;
			
			// "remove" the old item
			shipStats->GetDirGunData()->rigs[slot] = nullptr;

			return item;
		}

		void DoMissileRigEquip(const MissileRigItem& missileRig, EntityID id, size_t slot)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			shipStats->GetMissileLauncherData()->LoadNewMissileRig(missileRig.rigName, slot);
		}

		Item DoMissileRigUnEquip(EntityID id, size_t slot)
		{
			// Get the item
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			const auto& rig = shipStats->GetMissileLauncherData()->rigs[slot];

			// Copy rig name
			auto item = ItemFactory::Create<ItemType::MissileRig>(1);
			item.rigName = rig->name;

			// "remove" the old item
			shipStats->GetMissileLauncherData()->rigs[slot] = nullptr;

			return item;
		}

		size_t GetNumLaserRigSlots(EntityID id)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			return shipStats->GetDirGunData()->rigs.size();
		}

		size_t GetNumMissileRigSlots(EntityID id)
		{
			const auto handle = EntityManager::Get(id);
			auto& shipStats = handle->GetComponent<ShipStatsComponent>();
			return shipStats->GetMissileLauncherData()->rigs.size();
		}
	}

	void Equip(const Item& item, EntityID id, size_t slot)
	{
		switch(item.type)
		{
		case ItemType::LaserRig: 
			DoLaserRigEquip(ItemFactory::Create<ItemType::LaserRig>(1, static_cast<const LaserRigItem*>(&item)->rigName), id, slot);
			break;
		case ItemType::MissileRig:
			DoMissileRigEquip(ItemFactory::Create<ItemType::MissileRig>(1, static_cast<const MissileRigItem*>(&item)->rigName), id, slot);
			break;
		default: 
			throw "unrecognized item type in call to Equipper::Equip";
		}
	}

	Item Unequip(ItemType type, EntityID id, size_t slot)
	{
		switch (type)
		{
		case ItemType::LaserRig:
			return DoLaserRigUnEquip(id, slot);
		case ItemType::MissileRig:
			return DoMissileRigUnEquip(id, slot);
		default:
			throw "unrecognized item type in call to Equipper::UnEquip";
		}
	}

	size_t GetNumSlots(ItemType type, EntityID id)
	{
		switch (type)
		{
		case ItemType::LaserRig:
			return GetNumLaserRigSlots(id);
		case ItemType::MissileRig:
			return GetNumMissileRigSlots(id);
		default:
			throw "unrecognized item type in call to Equipper::GetNumSlots";
		}
	}
}