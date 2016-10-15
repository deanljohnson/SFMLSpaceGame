#include <UI/ShipNameEntry.h>
#include "UI.h"
#include <SFGUI/Button.hpp>
#include <SFGUI/Entry.hpp>

ShipNameEntry::ShipNameEntry()
	: GameWindow("ship_name_entry")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select New Ship Name");
	m_window->SetId("ship_name_entry");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	m_table = sfg::Table::Create();
	m_window->Add(m_table);

	auto nameEntry = sfg::Entry::Create();
	nameEntry->SetRequisition(sf::Vector2f(100, 0));
	m_table->Attach(nameEntry, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	m_table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, nameEntry] { CallCallback(nameEntry->GetText()); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

void ShipNameEntry::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}

void ShipNameEntry::SetupWindowSignals()
{
	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect([this] { OnMouseEnter(); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect([this] { OnMouseLeave(); });
	m_window->GetSignal(sfg::Window::OnCloseButton).Connect([this] { Show(false); });
}

void ShipNameEntry::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}

void ShipNameEntry::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
		return;

	m_window->Show(val);

	if (m_containsMouse && !val)
		OnMouseLeave();
}

bool ShipNameEntry::IsShown()
{
	return m_window->IsLocallyVisible();
}