// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/StationWindow.h>
#include <UI/StationTradeWindow.h>
#include "UI.h"
#include "ShipSelector.h"
#include <PlayerData.h>
#include <EntityManager.h>
#include <EntityHandle.h>
#include <Entity.h>
#include <StationStats.h>
#include <Components/StationStatsComponent.h>

struct StationWindow::StationWindowImpl 
{
private:
	size_t m_curRow;
	StationWindow* m_parent;
public:
	StationWindowImpl(StationWindow* parent) 
		: m_parent(parent), m_curRow(0)
	{
	}

	sfg::Table::Ptr m_table;

	void ClearTable() 
	{
		m_curRow = 0;
		m_table->RemoveAll();
	}
	void AddShipEditorButton() 
	{
		auto shipEditButton = sfg::Button::Create("Open Ship Editor");
		m_table->Attach(shipEditButton, { 0, m_curRow, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
		m_curRow++;

		shipEditButton->GetSignal(sfg::Button::OnLeftClick).Connect(
			[this] { GetWindow("ship_editor")->Show(true); m_parent->Show(false); });
	}
	void AddRigEditorButton() 
	{
		auto rigEditButton = sfg::Button::Create("Open Rig Editor");
		m_table->Attach(rigEditButton, { 0, m_curRow, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
		m_curRow++;

		rigEditButton->GetSignal(sfg::Button::OnLeftClick).Connect(
			[this] { GetWindow("rig_editor")->Show(true); m_parent->Show(false); });
	}
	void AddChangeShipButton() 
	{
		auto changeShipButton = sfg::Button::Create("Change Ship");
		m_table->Attach(changeShipButton, { 0, m_curRow, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
		m_curRow++;

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
				m_parent->Show(false);
			});
	}
	void AddTradeButton() 
	{
		auto tradeButton = sfg::Button::Create("Trade");
		m_table->Attach(tradeButton, { 0, m_curRow, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
		m_curRow++;

		tradeButton->GetSignal(sfg::Button::OnLeftClick).Connect(
			[this]
			{
				auto tradeWindow = GetWindow<StationTradeWindow>("station_trade");
				tradeWindow->SetTarget(m_parent->m_target);
				tradeWindow->Show(true);
				m_parent->Show(false);
			});
	}
};

StationWindow::StationWindow()
	: GameWindow("station_window"),
	m_impl(std::make_unique<StationWindowImpl>(this))
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Option");
	m_window->SetId("station_window");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	CenterOnScreen();

	m_impl->m_table = sfg::Table::Create();
	m_window->Add(m_impl->m_table);
}

StationWindow::~StationWindow() = default;

void StationWindow::SetTarget(EntityID targetID) 
{
	m_target = targetID;

	auto& stationStats = EntityManager::Get(m_target)->GetComponent<StationStatsComponent>();

	m_impl->ClearTable();

	if (stationStats->hasShipEditor)
		m_impl->AddShipEditorButton();
	if (stationStats->hasRigEditor)
		m_impl->AddRigEditorButton();
	if (stationStats->hasChangeShip)
		m_impl->AddChangeShipButton();
	if (stationStats->hasTrade)
		m_impl->AddTradeButton();
}