#pragma once

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
		Credits, FuelCells
	};

	struct Credits : public ItemBase<Credits>
	{
	};

	struct FuelCells : public ItemBase<FuelCells>
	{
	};

	union
	{
		Credits credits;
		FuelCells fuelCells;
	};

	ItemType type;

	void Stack(Item other);

	static Item Create(ItemType type, unsigned int amount);
};