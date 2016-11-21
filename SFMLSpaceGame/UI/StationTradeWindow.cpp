#include "stdafx.h"
#include <UI\StationTradeWindow.h>
#include <UI\UI.h>
#include <PlayerData.h>
#include <EntityManager.h>
#include <Components/Inventory.h>
#include <Economy.h>
#include <Components/EconomyAgent.h>

StationTradeWindow::StationTradeWindow() 
	: GameWindow("station_trade")
{
	static const sf::Vector2f scaleSize{ 80, 25 };

	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Station Trade");
	m_window->SetId("station_trade");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto stationSideBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto playerSideBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto centralBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);

	m_stationHeaderLabel = sfg::Label::Create("Station");
	stationSideBox->Pack(m_stationHeaderLabel);
	stationSideBox->Pack(m_stationInvenWidget.GetWidget());

	m_buyButton = sfg::Button::Create("Buy");
	centralBox->Pack(m_buyButton, false, false);
	m_buyAmountLabel = sfg::Label::Create("+0");
	centralBox->Pack(m_buyAmountLabel);
	m_buyScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
	auto buyAdjust = m_buyScale->GetAdjustment();
	buyAdjust->SetLower(0);
	buyAdjust->SetUpper(100);
	buyAdjust->SetMinorStep(1.f);
	buyAdjust->SetMajorStep(10.f);
	m_buyScale->SetRequisition(scaleSize);
	// This box keeps the slider control from scaling wierdly
	auto buyScaleBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	buyScaleBox->Pack(m_buyScale, false, false);
	centralBox->Pack(buyScaleBox);

	m_sellButton = sfg::Button::Create("Sell");
	centralBox->Pack(m_sellButton, false, false);
	m_sellAmountLabel = sfg::Label::Create("-0");
	centralBox->Pack(m_sellAmountLabel);
	m_sellScale = sfg::Scale::Create(sfg::Scale::Orientation::HORIZONTAL);
	auto sellAdjust = m_sellScale->GetAdjustment();
	sellAdjust->SetLower(0);
	sellAdjust->SetUpper(100);
	sellAdjust->SetValue(100);
	sellAdjust->SetMinorStep(1.f);
	sellAdjust->SetMajorStep(10.f);
	m_sellScale->SetRequisition(scaleSize);
	// This box keeps the slider control from scaling wierdly
	auto scalebox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	scalebox->Pack(m_sellScale, false, false);
	centralBox->Pack(scalebox);
	
	m_playerHeaderLabel = sfg::Label::Create("Your Ship");
	playerSideBox->Pack(m_playerHeaderLabel);
	playerSideBox->Pack(m_playerInvenWidget.GetWidget());
	
	mainBox->Pack(stationSideBox);
	mainBox->Pack(centralBox);
	mainBox->Pack(playerSideBox);

	m_window->Add(mainBox);

	m_stationInvenWidget.AddItemSelectionChangeCallback([this](Item* i) { OnBuyItemChange(i); });
	m_playerInvenWidget.AddItemSelectionChangeCallback([this](Item* i) { OnSellItemChange(i); });
	m_buyButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnBuyClick(); });
	m_sellButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnSellClick(); });
	buyAdjust->GetSignal(sfg::Adjustment::OnChange).Connect([this] { OnBuyScaleChange(); });
	sellAdjust->GetSignal(sfg::Adjustment::OnChange).Connect([this] { OnSellScaleChange(); });
}

void StationTradeWindow::Update()
{
	if (m_buyLabelNeedsUpdate)
	{
		auto buyAmount = static_cast<int>(m_buyScale->GetValue());
		std::stringstream ss;
		ss << "+" << buyAmount;
		m_buyAmountLabel->SetText(ss.str());

		m_buyLabelNeedsUpdate = false;
	}
	if (m_sellLabelNeedsUpdate)
	{
		auto sellAmount = static_cast<int>(m_sellScale->GetValue());
		sellAmount = m_sellScale->GetAdjustment()->GetUpper() - sellAmount;
		std::stringstream ss;
		ss << "-" << sellAmount;
		m_sellAmountLabel->SetText(ss.str());

		m_sellLabelNeedsUpdate = false;
	}
}

void StationTradeWindow::SetTarget(EntityID targetID) 
{
	m_target = targetID;
	auto& econAgent = EntityManager::Get(m_target)->GetComponent<EconomyAgent>();

	// Assign a price supplier to the inventory widgets so they can display
	// prices and differences from baseline
	m_stationInvenWidget.SetPriceSupplier(econAgent.GetSellPrices());
	m_playerInvenWidget.SetPriceSupplier(econAgent.GetBuyPrices());
	SetInventoryTargets();
}

void StationTradeWindow::OnBuyScaleChange()
{
	m_buyLabelNeedsUpdate = true;
}

void StationTradeWindow::OnSellScaleChange()
{
	m_sellLabelNeedsUpdate = true;
}

void StationTradeWindow::OnBuyItemChange(Item* item)
{
	auto adjustment = m_buyScale->GetAdjustment();
	auto amt = item == nullptr ? 0 : item->GetAmount();

	if (amt != 0) 
	{
		auto playerHandle = EntityManager::Get(PlayerData::GetActive()->GetID());
		auto& playerInven = playerHandle->GetComponent<Inventory>();
		auto playerCredits = playerInven.GetCredits();

		auto& stationAgent = EntityManager::Get(m_target)->GetComponent<EconomyAgent>();
		auto sellPrice = stationAgent.GetSellPrice(item->type);

		amt = std::min(amt, playerCredits / sellPrice);
	}

	adjustment->SetUpper(amt);
	adjustment->SetValue(0);
}

void StationTradeWindow::OnSellItemChange(Item* item)
{
	auto adjustment = m_sellScale->GetAdjustment();
	auto amt = item == nullptr ? 0 : item->GetAmount();

	adjustment->SetUpper(amt);
	adjustment->SetValue(amt);
}

void StationTradeWindow::OnBuyClick()
{
	auto selectedItem = m_stationInvenWidget.GetSelected();
	if (selectedItem == nullptr) return;

	// Create an item to represent the transaction
	auto boughtItem = Item(selectedItem->type,
							static_cast<unsigned int>(m_buyScale->GetValue()));

	auto& stationAgent = EntityManager::Get(m_target)->GetComponent<EconomyAgent>();
	auto playerID = PlayerData::GetActive()->GetID();
	auto& playerAgent = EntityManager::Get(playerID)->GetComponent<EconomyAgent>();

	Economy::DoSell(stationAgent, playerAgent, boughtItem);

	SetInventoryTargets();
	ResetScales();
}

void StationTradeWindow::OnSellClick()
{
	auto selectedItem = m_playerInvenWidget.GetSelected();
	if (selectedItem == nullptr) return;

	// Create an item to represent the transaction
	auto soldItem = Item(selectedItem->type, 
						static_cast<unsigned int>(m_sellScale->GetAdjustment()->GetUpper() - m_sellScale->GetValue()));

	auto& stationAgent = EntityManager::Get(m_target)->GetComponent<EconomyAgent>();
	auto playerID = PlayerData::GetActive()->GetID();
	auto& playerAgent = EntityManager::Get(playerID)->GetComponent<EconomyAgent>();

	Economy::DoBuy(playerAgent, stationAgent, soldItem);

	SetInventoryTargets();
	ResetScales();
}

void StationTradeWindow::SetInventoryTargets() 
{
	m_stationInvenWidget.SetTarget(m_target);
	m_playerInvenWidget.SetTarget(PlayerData::GetActive()->GetID());
}

void StationTradeWindow::ResetScales() 
{
	auto selectedItem = m_stationInvenWidget.GetSelected();
	OnBuyItemChange(selectedItem);
	selectedItem = m_playerInvenWidget.GetSelected();
	OnSellItemChange(selectedItem);

	m_buyScale->Invalidate();
	m_sellScale->Invalidate();

	m_buyLabelNeedsUpdate = true;
	m_sellLabelNeedsUpdate = true;
}