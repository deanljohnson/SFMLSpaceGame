#pragma once
#include <string>

#ifndef NAMED_ITEM
#define NAMED_ITEM(s) static std::string GetTypeName() { static const std::string name = s; return name; }
#endif

class Item
{
private:
	void SetAmount(unsigned int amount);
public:
	template <typename T>
	struct ItemBase 
	{
		unsigned int amount;
		void Stack(T other) { amount += other.amount; }
	};

	enum class ItemType
	{
		Credits, FuelCells, Ore, Food, Narcotics
	};

	struct Credits : public ItemBase<Credits>
	{
		NAMED_ITEM("Credits")
	};

	struct FuelCells : public ItemBase<FuelCells>
	{
		NAMED_ITEM("Fuel Cells")
	};

	struct Ore : public ItemBase<Ore>
	{
		NAMED_ITEM("Ore")
	};

	struct Food : public ItemBase<Food>
	{
		NAMED_ITEM("Food")
	};

	struct Narcotics : public ItemBase<Narcotics>
	{
		NAMED_ITEM("Narcotics")
	};

	union
	{
		Credits credits;
		FuelCells fuelCells;
		Ore ore;
		Food food;
		Narcotics narcotics;
	};

	ItemType type;

	void Stack(Item other);
	std::string GetTypeName();

	static Item Create(ItemType type, unsigned int amount);
};