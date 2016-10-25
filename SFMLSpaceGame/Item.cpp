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
	case ItemType::Ore:
		ore.Stack(other.ore);
		break;
	}
}

std::string Item::GetTypeName() 
{
	switch (type)
	{
	case ItemType::Credits:
		return Credits::GetTypeName();
	case ItemType::FuelCells:
		return FuelCells::GetTypeName();
	case ItemType::Ore:
		return Ore::GetTypeName();
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
	case ItemType::Ore:
		ore.amount = amount;
		break;
	}
}