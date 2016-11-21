#pragma once
#include <string>
#include <cereal\access.hpp>

#ifndef NAMED_ITEM
#define NAMED_ITEM(s) static std::string GetTypeName() { static const std::string name = s; return name; }
#endif

enum class ItemType
{
	Credits, FuelCells, Ore, Food, Narcotics
};

class Item
{
public:
	template <typename T>
	struct ItemBase 
	{
		unsigned int amount;
		void Stack(T other) { amount += other.amount; }
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
	std::string GetTypeName() const;
	unsigned int GetAmount() const;
	void SetAmount(unsigned int amount);

	Item(ItemType type, unsigned int amount);
	Item();

private:
	friend class cereal::access;

	template<class Archive>
	void save(Archive& ar) const
	{
		ar(type, GetAmount());
	}

	template <class Archive>
	void load(Archive& ar)
	{
		unsigned int amount;
		ar(type, amount);
		SetAmount(amount);
	}
};