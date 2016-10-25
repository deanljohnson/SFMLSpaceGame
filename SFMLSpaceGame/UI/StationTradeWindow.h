#pragma once
#include <UI\GameWindow.h>
#include <UI\InventoryWidget.h>

class StationTradeWindow : public GameWindow 
{
private:
	EntityID m_target;

	InventoryWidget m_playerInvenWidget;
public:
	StationTradeWindow();

	void SetTarget(EntityID target);
};