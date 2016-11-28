#include "stdafx.h"
#include <UI/MissileRigEditorWidget.h>
#include <UI/UIDefaults.h>
#include <UI/GameWindow.h>
#include <MissileRig.h>
#include <ResourceLoader.h>
#include <Serializer.h>

MissileRigEditorWidget::MissileRigEditorWidget(const std::string& name)
	: m_window(sfg::Window::Create(sfg::Window::NO_STYLE)),
	m_missileRig(LoadRig<MissileRig>(name))
{
	auto table = sfg::Table::Create();
	table->SetRowSpacings(2.f);
	table->SetColumnSpacings(2.f);

	auto fireRateLabel = sfg::Label::Create("Fire Rate");
	auto thrustLabel = sfg::Label::Create("Thrust");
	auto damageLabel = sfg::Label::Create("Damage");

	m_fireRateEntry = sfg::Entry::Create();
	m_thrustEntry = sfg::Entry::Create();
	m_damageEntry = sfg::Entry::Create();

	m_fireRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_thrustEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_damageEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	table->Attach(fireRateLabel, { 0, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(thrustLabel, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(damageLabel, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_fireRateEntry, { 1, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_thrustEntry, { 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_damageEntry, { 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
	SetupEntryFocusSignals();

	m_fireRateEntry->SetText(std::to_string(m_missileRig->fireRate));
	m_thrustEntry->SetText(std::to_string(m_missileRig->missile->thrust));
	m_damageEntry->SetText(std::to_string(m_missileRig->missile->damage));

	m_window->Add(table);
}

void MissileRigEditorWidget::SetupEntryValidationSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_fireRateEntry); });
	m_thrustEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_thrustEntry); });
	m_damageEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_damageEntry); });
}

void MissileRigEditorWidget::SetupEntryFocusSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_thrustEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_damageEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);

	m_fireRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_thrustEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_damageEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
}

void MissileRigEditorWidget::ClearEditing()
{
	m_missileRig = nullptr;

	m_fireRateEntry->SetText("");
	m_thrustEntry->SetText("");
	m_damageEntry->SetText("");
}

bool MissileRigEditorWidget::CheckAllEntryValidity()
{
	const std::string valid = "valid";
	return m_fireRateEntry->GetId() == valid
		&& m_thrustEntry->GetId() == valid
		&& m_damageEntry->GetId() == valid;
}

void MissileRigEditorWidget::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	entry->SetId(WidgetHelpers::ValidateFloatEntry(entry)
		? "valid"
		: "invalid");
}

void MissileRigEditorWidget::Save()
{
	if (!CheckAllEntryValidity())
		return;

	m_missileRig->fireRate = stof(m_fireRateEntry->GetText().toAnsiString());
	m_missileRig->missile->thrust = stof(m_thrustEntry->GetText().toAnsiString());
	m_missileRig->missile->damage = stof(m_damageEntry->GetText().toAnsiString());

	Serializer<> ser;
	ser.Save(m_missileRig.get(), m_missileRig->name, m_missileRig->name);
}

void MissileRigEditorWidget::Delete()
{
	assert(m_missileRig != nullptr);

	Serializer<> ser;
	ser.DeleteRecord(m_missileRig.get(), m_missileRig->name);
	ClearEditing();
}

sfg::Widget::Ptr MissileRigEditorWidget::GetWidget()
{
	return m_window;
}