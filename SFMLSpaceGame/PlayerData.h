#pragma once
#include <memory>
#include <functional>
#include <EntityID.h>

class PlayerData
{
private:
	static std::shared_ptr<PlayerData> m_activeData;

	EntityID m_ID;
	std::string m_shipID;
	std::function<void(const std::string&)> m_shipChangeCallback;
public:
	explicit PlayerData(const std::string& shipID);

	void SetPlayerShip(const std::string& shipID);
	std::string GetPlayerShip() const;

	void SetShipChangeCallback(std::function<void(const std::string&)> callback);
	void SetID(EntityID id);
	EntityID GetID() const;
	static std::shared_ptr<PlayerData> GetActive();
	static void SetActive(std::shared_ptr<PlayerData> pd);
};