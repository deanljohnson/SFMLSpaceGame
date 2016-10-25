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
	case ItemType::Food:
		food.Stack(other.food);
		break;
	case ItemType::Narcotics:
		narcotics.Stack(other.narcotics);
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
	case ItemType::Food:
		return Food::GetTypeName();
	case ItemType::Narcotics:
		return Narcotics::GetTypeName();
	}
}

Item Item::Create(ItemType type, unsigned int amount)
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
	case ItemType::Food:
		food.amount = amount;
		break;
	case ItemType::Narcotics:
		narcotics.amount = amount;
		break;
	}
}