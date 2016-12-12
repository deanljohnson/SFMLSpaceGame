#pragma once
#include <Item.h>

namespace ItemFactory
{
	// simplifies syntax
	template<bool Test, typename A, typename B>
	using Case = typename std::conditional<Test, A, B>::type;

	// Essentially a compile time switch statement
	template<ItemType T>
	struct ItemTypeType_
	{
		using type = Case<T == ItemType::LaserRig, LaserRigItem,
					Case<T == ItemType::Credits, CreditsItem,
					Case<T == ItemType::Food, FoodItem,
					Case<T == ItemType::FuelCells, FuelCellsItem,
					Case<T == ItemType::Narcotics, NarcoticsItem, 
					Case<T == ItemType::Ore, OreItem,
					Case<T == ItemType::MissileRig, MissileRigItem,
					void>>>>>>>;
	};
	
	// simplifies syntax
	template<ItemType T>
	using ItemTypeType = typename ItemTypeType_<T>::type;

	// Creates a strongly typed item based on the given ItemType enumerated value
	// and forwards the given args to that types constructor
	template<ItemType T, typename... Args>
	ItemTypeType<T> Create(unsigned int amount, Args... args)
	{
		return ItemTypeType<T>(amount, std::forward<Args>(args)...);
	}

	// non-templated item creation. Use only if type is not known at compile time
	inline Item Create(ItemType type, unsigned int amount)
	{
		switch (type)
		{
		case ItemType::Credits:
			return CreditsItem(amount);
		case ItemType::FuelCells:
			return FuelCellsItem(amount);
		case ItemType::Ore:
			return OreItem(amount);
		case ItemType::Food:
			return FoodItem(amount);
		case ItemType::Narcotics:
			return NarcoticsItem(amount);
		case ItemType::LaserRig:
			return LaserRigItem(amount);
		case ItemType::MissileRig:
			return MissileRigItem(amount);
		}

		throw "unrecognized item type " + std::to_string(static_cast<std::underlying_type<ItemType>::type>(type));
	}

	// Creates a new item cloned from the given item
	inline Item Create(const Item* item)
	{
		switch (item->type)
		{
		case ItemType::Credits:
			return CreditsItem(item->amount);
		case ItemType::FuelCells:
			return FuelCellsItem(item->amount);
		case ItemType::Ore:
			return OreItem(item->amount);
		case ItemType::Food:
			return FoodItem(item->amount);
		case ItemType::Narcotics:
			return NarcoticsItem(item->amount);
		case ItemType::LaserRig:
			return LaserRigItem(item->amount, static_cast<const LaserRigItem*>(item)->rigName);
		case ItemType::MissileRig:
			return MissileRigItem(item->amount, static_cast<const MissileRigItem*>(item)->rigName);
		}

		throw "unrecognized item type " + std::to_string(static_cast<std::underlying_type<ItemType>::type>(item->type));
	}
};