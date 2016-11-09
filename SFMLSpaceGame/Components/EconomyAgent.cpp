#include "stdafx.h"
#include <Components/EconomyAgent.h>
#include <Components/Inventory.h>
#include <Economy.h>
#include <Entity.h>

EconomyAgent::EconomyAgent(EntityID ent)
	: Component(ent),
	  m_id(EconomyID::Create()),
	  m_inventory(entity->GetComponent<Inventory>())
{
	Economy::AddAgent(*this);
}

EconomyAgent::~EconomyAgent()
{
	Economy::RemoveAgent(*this);
}
EconomyID EconomyAgent::GetEconomyID() const
{
	return m_id;
}

void EconomyAgent::AddItem(const Item& item)
{
	m_inventory.AddItem(item);
}

void EconomyAgent::RemoveItem(const Item& item)
{
	m_inventory.RemoveItem(item);
}

void EconomyAgent::GiveCredits(unsigned int credits) 
{
	m_inventory.SetCredits(m_inventory.GetCredits() + credits);
}

void EconomyAgent::TakeCredits(unsigned int credits)
{
	m_inventory.SetCredits(m_inventory.GetCredits() - credits);
}

bool EconomyAgent::Buys(ItemType itemType)
{
	return Economy::Buys(m_id, itemType);
}

bool EconomyAgent::Sells(ItemType itemType)
{
	return Economy::Sells(m_id, itemType);
}

unsigned EconomyAgent::GetBuyPrice(ItemType itemType)
{
	return Economy::GetBuyPrice(m_id, itemType);
}

unsigned EconomyAgent::GetSellPrice(ItemType itemType)
{
	return Economy::GetSellPrice(m_id, itemType);
}

void EconomyAgent::SetBuyPrice(ItemType itemType, Price price)
{
	Economy::SetBuyPrice(m_id, itemType, price);
}

void EconomyAgent::SetSellPrice(ItemType itemType, Price price)
{
	Economy::SetSellPrice(m_id, itemType, price);
}

void EconomyAgent::SetBuyPrices(std::initializer_list<std::pair<ItemType, Price>> prices)
{
	for (auto& p : prices)
	{
		SetBuyPrice(p.first, p.second);
	}
}

void EconomyAgent::SetSellPrices(std::initializer_list<std::pair<ItemType, Price>> prices)
{
	for (auto& p : prices)
	{
		SetSellPrice(p.first, p.second);
	}
}

PriceSupplier EconomyAgent::GetBuyPrices()
{
	return PriceSupplier(m_id, true);
}

PriceSupplier EconomyAgent::GetSellPrices()
{
	return PriceSupplier(m_id, false);
}