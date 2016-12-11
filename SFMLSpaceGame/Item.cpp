// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Item.h>
#include <assert.h>

const std::string Item::NO_DETAIL = "";

Item::Item()
	: type(ItemType::Credits),
	  amount(0)
{
}

Item::Item(ItemType _type, unsigned _amt)
	: type(_type), amount(_amt)
{
}

void Item::Stack(const Item& other)
{
	assert(type == other.type);
	
	amount += other.amount;
}

bool Item::AreStackable(const Item& other)
{
	return type == other.type
		&& (!IsDetailed()
			|| GetDetail() == other.GetDetail());
}

bool Item::IsDetailed() const
{
	return type == ItemType::LaserRig
		|| type == ItemType::MissileRig;
}

bool Item::IsEquippable() const 
{
	return type == ItemType::LaserRig
		|| type == ItemType::MissileRig;
}

std::string Item::GetTypeName() const
{
	switch (type)
	{
	case ItemType::Credits:
		return CreditsItem::GetTypeName();
	case ItemType::FuelCells:
		return FuelCellsItem::GetTypeName();
	case ItemType::Ore:
		return OreItem::GetTypeName();
	case ItemType::Food:
		return FoodItem::GetTypeName();
	case ItemType::Narcotics:
		return NarcoticsItem::GetTypeName();
	case ItemType::LaserRig:
		return LaserRigItem::GetTypeName();
	case ItemType::MissileRig:
		return MissileRigItem::GetTypeName();
	default:
		throw "unrecognized item type in GetTypeName call";
	}
}

std::string Item::GetDisplayString() const
{
	return GetTypeName();
}

const std::string& Item::GetDetail() const
{
	return NO_DETAIL;
}

std::string LaserRigItem::GetDisplayString() const
{
	return rigName;
}

const std::string& LaserRigItem::GetDetail() const
{
	return rigName;
}

std::string MissileRigItem::GetDisplayString() const
{
	return rigName;
}

const std::string& MissileRigItem::GetDetail() const
{
	return rigName;
}

// enable polymorphic item serialization
// include any archives we may use
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

// Include the polymorphic serialization and registration mechanisms
#include <cereal/types/polymorphic.hpp>

CEREAL_REGISTER_TYPE(CreditsItem);
CEREAL_REGISTER_TYPE(FuelCellsItem);
CEREAL_REGISTER_TYPE(OreItem);
CEREAL_REGISTER_TYPE(FoodItem);
CEREAL_REGISTER_TYPE(NarcoticsItem);
CEREAL_REGISTER_TYPE(LaserRigItem);
CEREAL_REGISTER_TYPE(MissileRigItem);