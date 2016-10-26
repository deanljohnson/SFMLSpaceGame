#pragma once
#include <UI\GameWindow.h>
#include <UI\InventoryWidget.h>
#include <SFGUI/Label.hpp>

class StationTradeWindow : public GameWindow 
{
private:
	EntityID m_target;

	sfg::Label::Ptr m_stationHeaderLabel;
	sfg::Label::Ptr m_playerHeaderLabel;

	InventoryWidget m_playerInvenWidget;
	InventoryWidget m_stationInvenWidget;
public:
	StationTradeWindow();

	void SetTarget(EntityID target);
};