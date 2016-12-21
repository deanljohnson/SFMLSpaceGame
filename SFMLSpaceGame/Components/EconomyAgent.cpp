// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/EconomyAgent.h>
#include <Components/Inventory.h>
#include <Entity.h>
#include <Economy.h>

EconomyAgent::EconomyAgent(EntityID ent, const EconomyID& id)
	: Component(ent),
	m_id(id),
	m_inventory(entity->GetComponent<Inventory>())
{
	Economy::AddAgent(*this);
}

EconomyAgent::EconomyAgent(EntityID ent, EconomyAgentType agentType)
	: Component(ent),
	  m_id(EconomyID::Create(agentType)),
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

void EconomyAgent::AddItem(std::shared_ptr<Item> item)
{
	m_inventory.AddItem(item);
}

void EconomyAgent::RemoveItem(std::shared_ptr<Item> item)
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

size_t EconomyAgent::GetAmountOfItem(ItemType type, const std::string& detail)
{
	for (auto& i : m_inventory)
	{
		if (i->type == type
			&& detail == i->GetDetail())
			return i->amount;
	}

	return 0;
}

Price EconomyAgent::GetBuyPrice(ItemType itemType, const std::string& detail)
{
	return Economy::GetBuyPrice(m_id, itemType, detail);
}

Price EconomyAgent::GetSellPrice(ItemType itemType, const std::string& detail)
{
	return Economy::GetSellPrice(m_id, itemType, detail);
}

PriceSupplier EconomyAgent::GetBuyPrices()
{
	return PriceSupplier(m_id, true);
}

PriceSupplier EconomyAgent::GetSellPrices()
{
	return PriceSupplier(m_id, false);
}

Inventory& EconomyAgent::GetInventory()
{
	return m_inventory;
}