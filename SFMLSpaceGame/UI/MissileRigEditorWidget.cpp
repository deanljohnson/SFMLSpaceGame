#include "stdafx.h"
#include <UI/MissileRigEditorWidget.h>
#include <UI/UIDefaults.h>
#include <UI/GameWindow.h>

MissileRigEditorWidget::MissileRigEditorWidget(const std::string& name)
	: m_window(sfg::Window::Create(sfg::Window::NO_STYLE)),
	m_missileRig(LoadRig<MissileRig>(name))
{
	auto table = sfg::Table::Create();
	table->SetRowSpacings(2.f);
	table->SetColumnSpacings(2.f);

	auto fireRateLabel = sfg::Label::Create("Fire Rate");

	m_fireRateEntry = sfg::Entry::Create();

	m_fireRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	table->Attach(fireRateLabel, { 0, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_fireRateEntry, { 1, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
	SetupEntryFocusSignals();

	m_fireRateEntry->SetText(std::to_string(m_missileRig->fireRate));

	m_window->Add(table);
}

void MissileRigEditorWidget::SetupEntryValidationSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_fireRateEntry); });
}

void MissileRigEditorWidget::SetupEntryFocusSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);

	m_fireRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
}

void MissileRigEditorWidget::ClearEditing()
{
	m_missileRig = nullptr;

	m_fireRateEntry->SetText("");
}

bool MissileRigEditorWidget::CheckAllEntryValidity()
{
	const std::string valid = "valid";
	return m_fireRateEntry->GetId() == valid;
}

void MissileRigEditorWidget::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	entry->SetId(WidgetHelpers::ValideFloatEntry(entry)
		? "valid"
		: "invalid");
}

void MissileRigEditorWidget::Save()
{
	if (!CheckAllEntryValidity())
		return;

	m_missileRig->fireRate = stof(m_fireRateEntry->GetText().toAnsiString());

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