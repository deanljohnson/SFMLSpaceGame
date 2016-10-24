#include <Item.h>
#include <assert.h>

void Item::Stack(Item other)
{
	assert(type == other.type);

	switch (other.type)
	{
	case ItemType::Credits:
		credits.Stack(other.credits);
		break;
	case ItemType::FuelCells:
		fuelCells.Stack(other.fuelCells);
		break;
	}
}

Item Item::Create(Item::ItemType type, unsigned int amount)
{
	Item i;
	i.type = type;
	i.SetAmount(amount);

	return i;
}

void Item::SetAmount(unsigned int amount) 
{
	switch (type)
	{
	case ItemType::Credits:
		credits.amount = amount;
		break;
	case ItemType::FuelCells:
		fuelCells.amount = amount;
		break;
	}
}