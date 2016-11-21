#pragma once
#include <UI\GameWindow.h>
#include <UI\InventoryWidget.h>
#include <SFGUI\Widgets.hpp>

class StationTradeWindow : public GameWindow 
{
private:
	EntityID m_target;

	sfg::Label::Ptr m_stationHeaderLabel;
	sfg::Label::Ptr m_playerHeaderLabel;

	sfg::Scale::Ptr m_buyScale;
	sfg::Button::Ptr m_buyButton;
	sfg::Label::Ptr m_buyAmountLabel;

	sfg::Scale::Ptr m_sellScale;
	sfg::Button::Ptr m_sellButton;
	sfg::Label::Ptr m_sellAmountLabel;
	
	InventoryWidget m_stationInvenWidget;
	InventoryWidget m_playerInvenWidget;

	bool m_buyLabelNeedsUpdate;
	bool m_sellLabelNeedsUpdate;

	void OnBuyScaleChange();
	void OnSellScaleChange();

	void OnBuyItemChange(Item* item);
	void OnSellItemChange(Item* item);

	void OnBuyClick();
	void OnSellClick();

	void SetInventoryTargets();
	void ResetScales();

	unsigned GetPlayerCredits();
	void UpdatePlayerHeader();
public:
	StationTradeWindow();

	virtual void Update() override;

	void SetTarget(EntityID target);
};