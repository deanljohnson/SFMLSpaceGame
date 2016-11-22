#include "stdafx.h"
#include <UI/LaserRigEditorWidget.h>
#include <UI/UIDefaults.h>
#include <UI/GameWindow.h>

LaserRigEditorWidget::LaserRigEditorWidget(const std::string& name)
	: m_window(sfg::Window::Create(sfg::Window::NO_STYLE)),
	  m_laserRig(LoadRig<LaserRig>(name))
{
	auto table = sfg::Table::Create();
	table->SetRowSpacings(2.f);
	table->SetColumnSpacings(2.f);

	auto fireRateLabel = sfg::Label::Create("Fire Rate");
	auto heatLimitLabel = sfg::Label::Create("Heat Limit");
	auto cooldownRateLabel = sfg::Label::Create("Cooldown Rate");
	auto heatGenerateLabel = sfg::Label::Create("Heat Generated");

	m_fireRateEntry = sfg::Entry::Create();
	m_heatLimitEntry = sfg::Entry::Create();
	m_cooldownRateEntry = sfg::Entry::Create();
	m_heatGeneratedEntry = sfg::Entry::Create();

	m_fireRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_heatLimitEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_cooldownRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_heatGeneratedEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	table->Attach(fireRateLabel,		{ 0, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(heatLimitLabel,		{ 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cooldownRateLabel,	{ 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(heatGenerateLabel,	{ 0, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_fireRateEntry,		{ 1, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_heatLimitEntry,		{ 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_cooldownRateEntry,	{ 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_heatGeneratedEntry,	{ 1, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
	SetupEntryFocusSignals();

	m_fireRateEntry->SetText(std::to_string(m_laserRig->fireRate));
	m_heatLimitEntry->SetText(std::to_string(m_laserRig->heatLimit));
	m_cooldownRateEntry->SetText(std::to_string(m_laserRig->cooldownRate));
	m_heatGeneratedEntry->SetText(std::to_string(m_laserRig->heatGenerated));

	m_window->Add(table);
}

void LaserRigEditorWidget::SetupEntryValidationSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_fireRateEntry); });
	m_heatLimitEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_heatLimitEntry); });
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_cooldownRateEntry); });
	m_heatGeneratedEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_heatGeneratedEntry); });
}

void LaserRigEditorWidget::SetupEntryFocusSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_heatLimitEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_heatGeneratedEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);

	m_fireRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_heatLimitEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_heatGeneratedEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
}

void LaserRigEditorWidget::ClearEditing()
{
	m_laserRig = nullptr;

	m_fireRateEntry->SetText("");
	m_heatLimitEntry->SetText("");
	m_cooldownRateEntry->SetText("");
	m_heatGeneratedEntry->SetText("");
}

bool LaserRigEditorWidget::CheckAllEntryValidity()
{
	const std::string valid = "valid";
	return m_fireRateEntry->GetId() == valid
		&& m_heatLimitEntry->GetId() == valid
		&& m_cooldownRateEntry->GetId() == valid
		&& m_heatGeneratedEntry->GetId() == valid;
}

void LaserRigEditorWidget::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	entry->SetId(WidgetHelpers::ValideFloatEntry(entry)
		? "valid"
		: "invalid");
}

void LaserRigEditorWidget::Save()
{
	if (!CheckAllEntryValidity())
		return;

	m_laserRig->fireRate = stof(m_fireRateEntry->GetText().toAnsiString());
	m_laserRig->heatLimit = stof(m_heatLimitEntry->GetText().toAnsiString());
	m_laserRig->cooldownRate = stof(m_cooldownRateEntry->GetText().toAnsiString());
	m_laserRig->heatGenerated = stof(m_heatGeneratedEntry->GetText().toAnsiString());

	Serializer<> ser;
	ser.Save(m_laserRig.get(), m_laserRig->name, m_laserRig->name);
}

void LaserRigEditorWidget::Delete()
{
	assert(m_laserRig != nullptr);

	Serializer<> ser;
	ser.DeleteRecord(m_laserRig.get(), m_laserRig->name);
	ClearEditing();
}

sfg::Widget::Ptr LaserRigEditorWidget::GetWidget()
{
	return m_window;
}
