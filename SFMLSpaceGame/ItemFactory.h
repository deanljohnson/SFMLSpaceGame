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
					void>>>>>>;
	};
	
	// simplifies syntax
	template<ItemType T>
	using ItemTypeType = typename ItemTypeType_<T>::type;

	// Creates a strongly typed item based on the given ItemType enumerated value
	// and forwards the given args to that types constructor
	template<ItemType T, typename... Args>
	std::shared_ptr<ItemTypeType<T>> Create(unsigned int amount, Args... args)
	{
		return std::make_shared<ItemTypeType<T>>(amount, std::forward<Args>(args)...);
	}

	// non-templated item creation. Use only if type is not known at compile time
	inline std::shared_ptr<Item> Create(ItemType type, unsigned int amount)
	{
		switch (type)
		{
		case ItemType::Credits:
			return std::make_shared<CreditsItem>(amount);
		case ItemType::FuelCells:
			return std::make_shared<FuelCellsItem>(amount);
		case ItemType::Ore:
			return std::make_shared<OreItem>(amount);
		case ItemType::Food:
			return std::make_shared<FoodItem>(amount);
		case ItemType::Narcotics:
			return std::make_shared<NarcoticsItem>(amount);
		case ItemType::LaserRig:
			return std::make_shared<LaserRigItem>(amount);
		}

		throw "unrecognized item type " + std::to_string(static_cast<std::underlying_type<ItemType>::type>(type));
	}

	// Creates a new item cloned from the given item
	inline std::shared_ptr<Item> Create(Item* item)
	{
		switch (item->type)
		{
		case ItemType::Credits:
			return std::make_shared<CreditsItem>(item->amount);
		case ItemType::FuelCells:
			return std::make_shared<FuelCellsItem>(item->amount);
		case ItemType::Ore:
			return std::make_shared<OreItem>(item->amount);
		case ItemType::Food:
			return std::make_shared<FoodItem>(item->amount);
		case ItemType::Narcotics:
			return std::make_shared<NarcoticsItem>(item->amount);
		case ItemType::LaserRig:
			return std::make_shared<LaserRigItem>(item->amount, static_cast<LaserRigItem*>(item)->rigName);
		}

		throw "unrecognized item type " + std::to_string(static_cast<std::underlying_type<ItemType>::type>(item->type));
	}
};