#include "stdafx.h"
#include <UI/RigTypeSelector.h>

RigTypeSelector::RigTypeSelector()
	: GameWindow("rig_type_selector")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Select Rig Type");
	m_window->SetId("rig_type_selector");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	auto selectionBox = sfg::ComboBox::Create();
	table->Attach(selectionBox, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectionBox->AppendItem("Laser");

	auto selectButton = sfg::Button::Create("Select");
	auto cancelButton = sfg::Button::Create("Cancel");
	table->Attach(selectButton, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cancelButton, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	selectButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, selectionBox] { CallCallback(selectionBox->GetSelectedText()); Show(false); });
	cancelButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(""); Show(false); });
}

void RigTypeSelector::CallCallback(const std::string& name)
{
	if (m_callback != nullptr)
		m_callback(name);
}

void RigTypeSelector::SetCallback(std::function<void(const std::string&)> callback)
{
	m_callback = callback;
}