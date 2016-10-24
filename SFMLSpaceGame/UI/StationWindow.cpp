#include <UI/StationWindow.h>
#include <UI/StationTradeWindow.h>
#include "UI.h"
#include <SFGUI/Button.hpp>
#include <SFGUI/Table.hpp>
#include "ShipSelector.h"
#include <PlayerData.h>

StationWindow::StationWindow()
	: GameWindow("station_window")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Option");
	m_window->SetId("station_window");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	CenterOnScreen();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	auto shipEditButton = sfg::Button::Create("Open Ship Editor");
	auto changeShipButton = sfg::Button::Create("Change Ship");
	auto tradeButton = sfg::Button::Create("Trade");

	table->Attach(shipEditButton, { 0, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(changeShipButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(tradeButton, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	shipEditButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { GetWindow("ship_editor")->Show(true); Show(false); });
	changeShipButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] 
		{ 
			auto selectWindow = static_cast<ShipSelector*>(GetWindow("ship_select"));
			selectWindow->SetCallback([this](const std::string& name)
			{
				if (name.empty()) return;
				PlayerData::GetActive()->SetPlayerShip(name);
			});
			selectWindow->Show(true);
			Show(false);
		});
	tradeButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] 
		{
			auto tradeWindow = GetWindow<StationTradeWindow>("station_trade");
			tradeWindow->SetTarget(m_target);
			tradeWindow->Show(true);
			Show(false);
		});
}

void StationWindow::SetTarget(EntityID targetID) 
{
	m_target = targetID;
}