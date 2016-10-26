#include <UI\StationTradeWindow.h>
#include <UI\UI.h>
#include <PlayerData.h>
#include <EntityManager.h>
#include <Components/Inventory.h>

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
	m_stationInvenWidget.SetTarget(m_target);
	m_playerInvenWidget.SetTarget(PlayerData::GetActive()->GetID());
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
	adjustment->SetUpper(item == nullptr ? 0 : item->GetAmount());
	adjustment->SetValue(0);
}

void StationTradeWindow::OnSellItemChange(Item* item)
{
	auto adjustment = m_sellScale->GetAdjustment();
	adjustment->SetUpper(item == nullptr ? 0 : item->GetAmount());
	adjustment->SetValue(item == nullptr ? 0 : item->GetAmount());
}

void StationTradeWindow::OnBuyClick()
{
	auto selectedItem = m_stationInvenWidget.GetSelected();
	if (selectedItem == nullptr) return;

	auto boughtItem = Item::Create(selectedItem->type,
									static_cast<unsigned int>(m_buyScale->GetValue()));

	auto playerID = PlayerData::GetActive()->GetID();
	auto& playerInventory = EntityManager::Get(playerID)->GetComponent<Inventory>();
	playerInventory.AddItem(boughtItem);

	auto& stationInventory = EntityManager::Get(m_target)->GetComponent<Inventory>();
	stationInventory.RemoveItem(boughtItem);

	// Forces a recreation of the widget. Not very 
	// efficient, but simple and works
	m_stationInvenWidget.SetTarget(m_target);
	m_playerInvenWidget.SetTarget(playerID);
}

void StationTradeWindow::OnSellClick()
{
	auto selectedItem = m_playerInvenWidget.GetSelected();
	if (selectedItem == nullptr) return;

	auto soldItem = Item::Create(selectedItem->type, 
								static_cast<unsigned int>(m_sellScale->GetAdjustment()->GetUpper() - m_sellScale->GetValue()));

	auto& stationInventory = EntityManager::Get(m_target)->GetComponent<Inventory>();
	stationInventory.AddItem(soldItem);

	auto playerID = PlayerData::GetActive()->GetID();
	auto& playerInventory = EntityManager::Get(playerID)->GetComponent<Inventory>();
	playerInventory.RemoveItem(soldItem);

	// Forces a recreation of the widget. Not very 
	// efficient, but simple and works
	m_stationInvenWidget.SetTarget(m_target);
	m_playerInvenWidget.SetTarget(PlayerData::GetActive()->GetID());
}