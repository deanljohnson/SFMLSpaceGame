#include <UI/ShipSelector.h>
#include "UI.h"
#include <FileSystem.h>
#include <SFGUI/ComboBox.hpp>
#include <SFGUI/Button.hpp>
#include <ShipStats.h>
#include <SFGUI/Table.hpp>

ShipSelector::ShipSelector()
	: GameWindow("ship_select")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Ship");
	m_window->SetId("ship_select");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_selectionBox = sfg::ComboBox::Create();
	table->Attach(m_selectionBox, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	std::vector<std::string> shipFiles;
	FileSystem::GetFileNamesWithExtension(DATA_PATH, "." + ShipStats::GetTypeName(), shipFiles);
	for (auto e : shipFiles)
	{
		// Strip the extension from the file name
		e.erase(e.find_last_of("."), std::string::npos);
		m_selectionBox->AppendItem(e);
	}

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(m_selectionBox->GetSelectedText()); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

void ShipSelector::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}

void ShipSelector::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}

void ShipSelector::Show(bool val)
{
	GameWindow::Show(val);

	if (val)
	{
		m_selectionBox->Clear();

		std::vector<std::string> shipFiles;
		FileSystem::GetFileNamesWithExtension(DATA_PATH, "." + ShipStats::GetTypeName(), shipFiles);
		for (auto e : shipFiles)
		{
			// Strip the extension from the file name
			e.erase(e.find_last_of("."), std::string::npos);
			m_selectionBox->AppendItem(e);
		}
	}
}
