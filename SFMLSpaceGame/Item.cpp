#include "stdafx.h"
#include <Item.h>
#include <assert.h>

Item::Item(ItemType _type, unsigned int _amount)
	: type(_type)
{
	SetAmount(_amount);
}

Item::Item(const Item& other)
	: Item(other.type, other.GetAmount())
{
	switch (other.type)
	{
	case ItemType::LaserRig:
		laserRig = LaserRigItem(other.laserRig.rigName);
		break;
	}
}

Item::Item()
	: laserRig(), type(ItemType::Credits)
{
	SetAmount(0);
}

Item::~Item()
{
	switch (type)
	{
	case ItemType::LaserRig:
		laserRig.~LaserRigItem();
		break;
	}
}

Item& Item::operator=(const Item& other)
{
	type = other.type;
	SetAmount(other.GetAmount());

	switch (type)
	{
	case ItemType::LaserRig:
		laserRig.rigName = other.laserRig.rigName;
		break;
	}

	return *this;
}

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
	case ItemType::LaserRig:
		laserRig.Stack(other.laserRig);
		break;
	}
}

std::string Item::GetTypeName() const
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
	case ItemType::LaserRig:
		return LaserRigItem::GetTypeName();
	}
}

unsigned Item::GetAmount() const
{
	switch (type)
	{
	case ItemType::Credits:
		return credits.amount;
	case ItemType::FuelCells:
		return fuelCells.amount;
	case ItemType::Ore:
		return ore.amount;
	case ItemType::Food:
		return food.amount;
	case ItemType::Narcotics:
		return narcotics.amount;
	case ItemType::LaserRig:
		return laserRig.amount;
	}
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
	case ItemType::LaserRig:
		laserRig.amount = amount;
		break;
	}
}