#include <UI/ShipSelector.h>
#include "UI.h"
#include <FileSystem.h>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Button.hpp>
#include <ShipStats.h>

ShipSelector::ShipSelector()
	: GameWindow("ship_select")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Ship");
	m_window->SetId("ship_select");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	m_table = sfg::Table::Create();
	m_window->Add(m_table);

	auto selectionBox = sfg::ComboBox::Create();
	m_table->Attach(selectionBox, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	std::vector<std::string> shipFiles;
	FileSystem::GetFileNamesWithExtension(DATA_PATH, "." + ShipStats::GetTypeName(), shipFiles);
	for (auto e : shipFiles)
	{
		// Strip the extension from the file
		e.erase(e.find_last_of("."), std::string::npos);
		selectionBox->AppendItem(e);
	}

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	m_table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, selectionBox] { CallCallback(selectionBox->GetSelectedText()); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

void ShipSelector::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}

void ShipSelector::SetupWindowSignals()
{
	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect([this]{ OnMouseEnter(); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect([this]{ OnMouseLeave(); });
	m_window->GetSignal(sfg::Window::OnCloseButton).Connect([this]{ Show(false); });
}

void ShipSelector::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}

void ShipSelector::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
		return;

	m_window->Show(val);

	if (m_containsMouse && !val)
		OnMouseLeave();
}

bool ShipSelector::IsShown()
{
	return m_window->IsLocallyVisible();
}