#include <UI\StationTradeWindow.h>
#include <UI\UI.h>

StationTradeWindow::StationTradeWindow() 
	: GameWindow("station_trade")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Station Trade");
	m_window->SetId("station_trade");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	m_playerInvenWidget.SetSize({5,5});

	m_window->Add(m_playerInvenWidget.GetWidget());
}

void StationTradeWindow::SetTarget(EntityID targetID) 
{
	m_target = targetID;
}