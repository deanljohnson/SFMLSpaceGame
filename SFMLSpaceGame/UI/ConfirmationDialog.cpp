#include "stdafx.h"
#include <UI/ConfirmationDialog.h>
#include <UI/UI.h>

ConfirmationDialog::ConfirmationDialog() 
	: GameWindow("confirmation_dialog")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL);
	m_window->SetTitle("Confirm");
	m_window->SetId("confirmation_dialog");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_label = sfg::Label::Create("Confirm?");
	m_yesButton = sfg::Button::Create("Yes");
	m_noButton = sfg::Button::Create("No");

	table->Attach(m_label, {0, 0, 2, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_yesButton, {0, 1, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_noButton, {1, 1, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	m_yesButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(true); Show(false); });
	m_noButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] { CallCallback(false); Show(false); });
}

void ConfirmationDialog::SetCallback(std::function<void(bool)> callback) 
{
	m_callback = callback;
}

void ConfirmationDialog::SetText(const std::string& text,
								const std::string& yes,
								const std::string& no) 
{
	m_label->SetText(text);
	m_yesButton->SetLabel(yes);
	m_noButton->SetLabel(no);
}

void ConfirmationDialog::CallCallback(bool result) 
{
	if (m_callback == nullptr)
		return;
	m_callback(result);
}