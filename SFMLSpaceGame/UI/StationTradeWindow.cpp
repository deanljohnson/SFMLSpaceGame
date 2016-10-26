#include <UI\StationTradeWindow.h>
#include <UI\UI.h>
#include <PlayerData.h>
#include <SFGUI\Box.hpp>
#include <SFGUI/Label.hpp>

StationTradeWindow::StationTradeWindow() 
	: GameWindow("station_trade"),
	  m_playerInvenWidget(true),
	  m_stationInvenWidget(true)
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Station Trade");
	m_window->SetId("station_trade");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto stationSideBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto playerSideBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	auto mainBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL, 10.f);

	m_stationHeaderLabel = sfg::Label::Create("Station");
	m_stationInvenWidget.SetSize({ 5,5 });
	stationSideBox->Pack(m_stationHeaderLabel);
	stationSideBox->Pack(m_stationInvenWidget.GetWidget());

	m_playerHeaderLabel = sfg::Label::Create("Your Ship");
	m_playerInvenWidget.SetSize({5,5});
	playerSideBox->Pack(m_playerHeaderLabel);
	playerSideBox->Pack(m_playerInvenWidget.GetWidget());
	
	mainBox->Pack(stationSideBox);
	mainBox->Pack(playerSideBox);

	m_window->Add(mainBox);
}

void StationTradeWindow::SetTarget(EntityID targetID) 
{
	m_target = targetID;
	m_stationInvenWidget.SetTarget(m_target);
	m_playerInvenWidget.SetTarget(PlayerData::GetActive()->GetID());
}