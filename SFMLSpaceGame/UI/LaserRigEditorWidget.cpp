#include "stdafx.h"
#include <UI/LaserRigEditorWidget.h>
#include <UI/UIDefaults.h>
#include <UI/GameWindow.h>
#include <LaserRig.h>
#include <ResourceLoader.h>
#include <Serializer.h>

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

	auto projHeader = sfg::Label::Create("Projectile Info");
	auto projSpeedLabel = sfg::Label::Create("Speed");
	auto projLifetimeLabel = sfg::Label::Create("Lifetime");
	auto projDamageLabel = sfg::Label::Create("Damage");
	auto projSizeXLabel = sfg::Label::Create("Size X");
	auto projSizeYLabel = sfg::Label::Create("Size Y");

	m_fireRateEntry = sfg::Entry::Create();
	m_heatLimitEntry = sfg::Entry::Create();
	m_cooldownRateEntry = sfg::Entry::Create();
	m_heatGeneratedEntry = sfg::Entry::Create();
	m_projSpeedEntry = sfg::Entry::Create();
	m_projLifetimeEntry = sfg::Entry::Create();
	m_projDamageEntry = sfg::Entry::Create();
	m_projSizeXEntry = sfg::Entry::Create();
	m_projSizeYEntry = sfg::Entry::Create();

	m_fireRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_heatLimitEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_cooldownRateEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_heatGeneratedEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_projSpeedEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_projLifetimeEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_projDamageEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_projSizeXEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_projSizeYEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	table->Attach(fireRateLabel,	{ 0, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(heatLimitLabel,	{ 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(cooldownRateLabel,{ 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(heatGenerateLabel,{ 0, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projHeader,		{ 0, 4, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projSpeedLabel,	{ 0, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projLifetimeLabel,{ 0, 6, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projDamageLabel,	{ 0, 7, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projSizeXLabel,	{ 0, 8, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(projSizeYLabel,	{ 0, 9, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	table->Attach(m_fireRateEntry,		{ 1, 0, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_heatLimitEntry,		{ 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_cooldownRateEntry,	{ 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_heatGeneratedEntry,	{ 1, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_projSpeedEntry,		{ 1, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_projLifetimeEntry,	{ 1, 6, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_projDamageEntry,	{ 1, 7, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_projSizeXEntry,		{ 1, 8, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_projSizeYEntry,		{ 1, 9, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
	SetupEntryFocusSignals();

	m_fireRateEntry->SetText(std::to_string(m_laserRig->fireRate));
	m_heatLimitEntry->SetText(std::to_string(m_laserRig->heatLimit));
	m_cooldownRateEntry->SetText(std::to_string(m_laserRig->cooldownRate));
	m_heatGeneratedEntry->SetText(std::to_string(m_laserRig->heatGenerated));
	m_projSpeedEntry->SetText(std::to_string(m_laserRig->projectile->speed));
	m_projLifetimeEntry->SetText(std::to_string(m_laserRig->projectile->lifeTime));
	m_projDamageEntry->SetText(std::to_string(m_laserRig->projectile->damage));
	m_projSizeXEntry->SetText(std::to_string(m_laserRig->projectile->size.x));
	m_projSizeYEntry->SetText(std::to_string(m_laserRig->projectile->size.y));

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

	m_projSpeedEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_projSpeedEntry); });
	m_projLifetimeEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_projLifetimeEntry); });
	m_projDamageEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_projDamageEntry); });
	m_projSizeXEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_projSizeXEntry); });
	m_projSizeYEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_projSizeYEntry); });
}

void LaserRigEditorWidget::SetupEntryFocusSignals()
{
	m_fireRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_heatLimitEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_heatGeneratedEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_projSpeedEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_projLifetimeEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_projDamageEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_projSizeXEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);
	m_projSizeYEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GameWindow::GrabKeyboardFocus);

	m_fireRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_heatLimitEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_heatGeneratedEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_projSpeedEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_projLifetimeEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_projDamageEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_projSizeXEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
	m_projSizeYEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(GameWindow::ReleaseKeyboardFocus);
}

void LaserRigEditorWidget::ClearEditing()
{
	m_laserRig = nullptr;

	m_fireRateEntry->SetText("");
	m_heatLimitEntry->SetText("");
	m_cooldownRateEntry->SetText("");
	m_heatGeneratedEntry->SetText("");

	m_projSpeedEntry->SetText("");
	m_projLifetimeEntry->SetText("");
	m_projDamageEntry->SetText("");
	m_projSizeXEntry->SetText("");
	m_projSizeYEntry->SetText("");
}

bool LaserRigEditorWidget::CheckAllEntryValidity()
{
	const std::string valid = "valid";
	return m_fireRateEntry->GetId() == valid
		&& m_heatLimitEntry->GetId() == valid
		&& m_cooldownRateEntry->GetId() == valid
		&& m_heatGeneratedEntry->GetId() == valid
		&& m_projSpeedEntry->GetId() == valid
		&& m_projLifetimeEntry->GetId() == valid
		&& m_projDamageEntry->GetId() == valid
		&& m_projSizeXEntry->GetId() == valid
		&& m_projSizeYEntry->GetId() == valid;
}

void LaserRigEditorWidget::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	entry->SetId(WidgetHelpers::ValidateFloatEntry(entry)
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

	m_laserRig->projectile->speed = stof(m_projSpeedEntry->GetText().toAnsiString());
	m_laserRig->projectile->lifeTime = stof(m_projLifetimeEntry->GetText().toAnsiString());
	m_laserRig->projectile->damage = stof(m_projDamageEntry->GetText().toAnsiString());
	m_laserRig->projectile->size.x = stof(m_projSizeXEntry->GetText().toAnsiString());
	m_laserRig->projectile->size.y = stof(m_projSizeYEntry->GetText().toAnsiString());

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
