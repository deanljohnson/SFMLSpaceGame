// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <PlayerData.h>

std::shared_ptr<PlayerData> PlayerData::m_activeData{ nullptr };

PlayerData::PlayerData(const std::string& shipName)
	: m_ID(ENTITY_ID_NULL),
	  m_shipName(shipName),
	  m_shipChangeCallback(nullptr)
{
}

void PlayerData::SetPlayerShip(const std::string& shipName)
{
	if (m_shipName == shipName)
		return;

	m_shipName = shipName;

	if (m_shipChangeCallback != nullptr)
		m_shipChangeCallback(m_shipName);
}

std::string PlayerData::GetPlayerShip() const
{
	return m_shipName;
}

void PlayerData::SetShipChangeCallback(std::function<void(const std::string&)> callback)
{
	m_shipChangeCallback = callback;
}

void PlayerData::SetID(EntityID id)
{
	m_ID = id;
}

EntityID PlayerData::GetID() const
{
	return m_ID;
}

std::shared_ptr<PlayerData> PlayerData::GetActive()
{
	return m_activeData;
}

void PlayerData::SetActive(std::shared_ptr<PlayerData> pd)
{
	m_activeData = pd;
}