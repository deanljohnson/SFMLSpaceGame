#pragma once
#include <UI\GameWindow.h>
#include <UI\PlayerInventoryWidget.h>
#include <Entity.h>

class StationTradeWindow : public GameWindow 
{
private:
	EntityID m_target;

	PlayerInventoryWidget m_playerInvenWidget;
public:
	StationTradeWindow();

	void SetTarget(EntityID target);
};