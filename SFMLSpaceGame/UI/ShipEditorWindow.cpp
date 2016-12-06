// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/ShipEditorWindow.h>
#include "UI.h"
#include <UI/UIDefaults.h>
#include "ShipSelector.h"
#include "ImageSelector.h"
#include "ShipNameEntry.h"
#include "HardPointEditor.h"
#include "ColliderEditor.h"
#include "ThrusterLocationEditor.h"
#include "ConfirmationDialog.h"
#include <Serializer.h>
#include <ShipStats.h>

ShipEditorWindow::ShipEditorWindow()
	: GameWindow("ship_editor")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Ship Editor");
	m_window->SetId("ship_editor");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	// Create widgets
	m_newShipButton = sfg::Button::Create("New Ship");
	m_editShipButton = sfg::Button::Create("Edit Ship");
	m_saveShipButton = sfg::Button::Create("Save Ship");
	m_deleteShipButton = sfg::Button::Create("Delete Ship");
	m_defineColliderButton = sfg::Button::Create("Edit Collider");
	m_hardpointEditorButton = sfg::Button::Create("Edit Hardpoints");
	m_thrusterEditorButton = sfg::Button::Create("Edit Thruster Locations");
	SetupButtonSignals();

	m_shipWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	m_shipWindow->Add(m_shipCanvas);
	SetupCanvasSignals();

	m_propertyTable = sfg::Table::Create();
	m_propertyTable->SetRowSpacings(2.f);
	m_propertyTable->SetColumnSpacings(2.f);
	SetupPropertyTable();

	// Create layout widgets
	m_topLevelBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_leftSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_rightSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_leftSideBar->Pack(m_newShipButton);
	m_leftSideBar->Pack(m_editShipButton);
	m_leftSideBar->Pack(m_saveShipButton);
	m_leftSideBar->Pack(m_deleteShipButton);

	m_mainBox->Pack(m_shipWindow);
	m_mainBox->Pack(m_defineColliderButton);
	m_mainBox->Pack(m_hardpointEditorButton);
	m_mainBox->Pack(m_thrusterEditorButton);

	m_rightSideBar->Pack(m_propertyTable);
	
	m_topLevelBox->Pack(m_leftSideBar);
	m_topLevelBox->Pack(m_mainBox);
	m_topLevelBox->Pack(m_rightSideBar);
	m_window->Add(m_topLevelBox);
}

void ShipEditorWindow::SetupButtonSignals()
{
	// Displays image selection window. The OnNewShipImageSelected function
	// will continue the new ship process
	m_newShipButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			auto selectWindow = static_cast<ImageSelector*>(GetWindow("image_select"));
			selectWindow->SetCallback([this](const std::string& name) { OnNewShipImageSelected(name); });
			selectWindow->Show(true);
		});

	m_editShipButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] 
		{ 
			auto selectWindow = static_cast<ShipSelector*>(GetWindow("ship_select"));
			selectWindow->SetCallback([this](const std::string& name) { OnShipSelected(name); });
			selectWindow->Show(true);
		});

	m_saveShipButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			OnSaveShip();
		});

	m_deleteShipButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			if (m_targetStats.get() == nullptr)
				return;
			auto editWindow = static_cast<ConfirmationDialog*>(GetWindow("confirmation_dialog"));
			editWindow->SetCallback([this](bool val) { if (val) OnDeleteShip(); });
			editWindow->SetText("Are you sure you want to delete this ship? This cannot be undone.");
			editWindow->CenterOnScreen(); // changing the text can make the window uncentered
			editWindow->Show(true);
		});

	m_defineColliderButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] 
		{ 
			if (m_targetStats.get() == nullptr)
				return;
			auto editWindow = static_cast<ColliderEditor*>(GetWindow("collider_editor"));
			editWindow->SetTarget(m_shipName, m_editingStats);
			editWindow->Show(true);
		});

	m_hardpointEditorButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			if (m_targetStats.get() == nullptr)
				return;
			auto editWindow = static_cast<HardPointEditor*>(GetWindow("hard_point_editor"));
			editWindow->SetTarget(m_shipName, m_editingStats);
			editWindow->Show(true);
		});

	m_thrusterEditorButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			if (m_targetStats.get() == nullptr)
				return;
			auto editWindow = static_cast<ThrusterLocationEditor*>(GetWindow("thruster_editor"));
			editWindow->SetTarget(m_shipName, m_editingStats);
			editWindow->Show(true);
		});
}

void ShipEditorWindow::SetupPropertyTable()
{
	auto aiHeaderLabel = sfg::Label::Create("AI Details");
	auto interLeadMultLabel = sfg::Label::Create("Intercept Lead Multiplier");
	auto followDistLabel = sfg::Label::Create("Follow Distance");
	auto approachDistLabel = sfg::Label::Create("Approach Distance");
	auto strafeDistLabel = sfg::Label::Create("Strafe Distance");
	auto sensorRangeLabel = sfg::Label::Create("Sensor Range");
	m_interLeadEntry = sfg::Entry::Create();
	m_followDistEntry = sfg::Entry::Create();
	m_approachDistEntry = sfg::Entry::Create();
	m_strafeDistEntry = sfg::Entry::Create();
	m_sensorRangeEntry = sfg::Entry::Create();

	m_interLeadEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_followDistEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_approachDistEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_strafeDistEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_sensorRangeEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	m_propertyTable->Attach(aiHeaderLabel,		{ 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(interLeadMultLabel, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(followDistLabel,	{ 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(approachDistLabel,	{ 0, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(strafeDistLabel,	{ 0, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sensorRangeLabel,	{ 0, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_interLeadEntry,	{ 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_followDistEntry,	{ 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_approachDistEntry,{ 1, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_strafeDistEntry,	{ 1, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_sensorRangeEntry,	{ 1, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto thrustHeaderLabel = sfg::Label::Create("Thrust Options");
	auto forwardThrustLabel = sfg::Label::Create("Forward");
	auto sideThrustLabel = sfg::Label::Create("Sideways");
	auto reverseThrustLabel = sfg::Label::Create("Reverse");
	auto steerThrustLabel = sfg::Label::Create("Steering");
	m_forwardThrustEntry = sfg::Entry::Create();
	m_sideThrustEntry = sfg::Entry::Create();
	m_reverseThrustEntry = sfg::Entry::Create();
	m_steerThrustEntry = sfg::Entry::Create();

	m_forwardThrustEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_sideThrustEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_reverseThrustEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_steerThrustEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	m_propertyTable->Attach(thrustHeaderLabel,	  { 0, 6,  2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(forwardThrustLabel,   { 0, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sideThrustLabel,	  { 0, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(reverseThrustLabel,	  { 0, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(steerThrustLabel,	  { 0, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_forwardThrustEntry, { 1, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_sideThrustEntry,	  { 1, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_reverseThrustEntry, { 1, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_steerThrustEntry,	  { 1, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto defenseHeaderLabel = sfg::Label::Create("Defense Stats");
	auto hullStrengthLabel = sfg::Label::Create("Hull Strength");
	auto forwardShieldLabel = sfg::Label::Create("Forward Shield Strength");
	auto sideShieldLabel = sfg::Label::Create("Side Shield Strength");
	auto rearShieldLabel = sfg::Label::Create("Rear Shield Strength");
	auto shieldRegenLabel = sfg::Label::Create("Shield Regen Speed");
	m_hullStrengthEntry = sfg::Entry::Create();
	m_frontShieldEntry = sfg::Entry::Create();
	m_sideShieldEntry = sfg::Entry::Create();
	m_rearShieldEntry = sfg::Entry::Create();
	m_shieldRegenEntry = sfg::Entry::Create();

	m_hullStrengthEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_frontShieldEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_sideShieldEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_rearShieldEntry->SetRequisition(UIDefaults::ENTRY_SIZE);
	m_shieldRegenEntry->SetRequisition(UIDefaults::ENTRY_SIZE);

	m_propertyTable->Attach(defenseHeaderLabel,	{ 2, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(hullStrengthLabel,	{ 2, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(forwardShieldLabel,	{ 2, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sideShieldLabel,	{ 2, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(rearShieldLabel,	{ 2, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(shieldRegenLabel,	{ 2, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_hullStrengthEntry,{ 3, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_frontShieldEntry,	{ 3, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_sideShieldEntry,	{ 3, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_rearShieldEntry,	{ 3, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_shieldRegenEntry,	{ 3, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
	SetupEntryFocusSignals();
}

void ShipEditorWindow::SetupCanvasSignals()
{
}

void ShipEditorWindow::SetupEntryValidationSignals()
{
	m_interLeadEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this]{ OnEntryFloatTextValidation(m_interLeadEntry); });
	m_followDistEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_followDistEntry); });
	m_approachDistEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_approachDistEntry); });
	m_strafeDistEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_strafeDistEntry); });
	m_sensorRangeEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_sensorRangeEntry); });
	m_forwardThrustEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_forwardThrustEntry); });
	m_sideThrustEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_sideThrustEntry); });
	m_reverseThrustEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_reverseThrustEntry); });
	m_steerThrustEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_steerThrustEntry); });
	m_hullStrengthEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_hullStrengthEntry); });
	m_frontShieldEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_frontShieldEntry); });
	m_sideShieldEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_sideShieldEntry); });
	m_rearShieldEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_rearShieldEntry); });
	m_shieldRegenEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_shieldRegenEntry); });
}

void ShipEditorWindow::SetupEntryFocusSignals()
{
	m_interLeadEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_followDistEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_approachDistEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_strafeDistEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_sensorRangeEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_forwardThrustEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_sideThrustEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_reverseThrustEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_steerThrustEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_hullStrengthEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_frontShieldEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_sideShieldEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_rearShieldEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);
	m_shieldRegenEntry->GetSignal(sfg::Entry::OnGainFocus).Connect(GrabKeyboardFocus);

	m_interLeadEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_followDistEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_approachDistEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_strafeDistEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_sensorRangeEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_forwardThrustEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_sideThrustEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_reverseThrustEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_steerThrustEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_hullStrengthEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_frontShieldEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_sideShieldEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_rearShieldEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
	m_shieldRegenEntry->GetSignal(sfg::Entry::OnLostFocus).Connect(ReleaseKeyboardFocus);
}

void ShipEditorWindow::LoadShipStatsToEntries()
{
	m_interLeadEntry->SetText(std::to_string(m_targetStats->interceptLeadMultiplier));
	m_followDistEntry->SetText(std::to_string(m_targetStats->followDistance));
	m_approachDistEntry->SetText(std::to_string(m_targetStats->approachDistance));
	m_strafeDistEntry->SetText(std::to_string(m_targetStats->strafeDistance));
	m_sensorRangeEntry->SetText(std::to_string(m_targetStats->sensorRange));

	auto thrust = m_targetStats->GetShipThrust();
	m_forwardThrustEntry->SetText(std::to_string(thrust->Forward));
	m_sideThrustEntry->SetText(std::to_string(thrust->Side));
	m_reverseThrustEntry->SetText(std::to_string(thrust->Reverse));
	m_steerThrustEntry->SetText(std::to_string(thrust->Steer));

	auto shields = m_targetStats->GetShieldData();
	m_hullStrengthEntry->SetText(std::to_string(m_targetStats->maxHull));
	m_frontShieldEntry->SetText(std::to_string(shields->FrontStrength));
	m_sideShieldEntry->SetText(std::to_string(shields->SideStrength));
	m_rearShieldEntry->SetText(std::to_string(shields->RearStrength));
	m_shieldRegenEntry->SetText(std::to_string(shields->RegenSpeed));
}

void ShipEditorWindow::LoadShipImage()
{
	m_shipTexture = LoadTexture(m_editingStats->imageLocation);
	m_shipImage.setTexture(*m_shipTexture.get());
	// Explicitly set the texture rect. Otherwise, changing ships can cause part of
	// the image to be occluded
	m_shipImage.setTextureRect({ 0, 0, static_cast<int>(m_shipTexture->getSize().x), static_cast<int>(m_shipTexture->getSize().y) });

	auto canvasSize = m_shipCanvas->GetRequisition();
	auto shipSize = sf::Vector2f(m_shipImage.getLocalBounds().width, m_shipImage.getLocalBounds().height);

	m_shipImage.setPosition((canvasSize / 2.f) - (shipSize / 2.f));
}

bool ShipEditorWindow::CheckAllEntryValidity()
{
	std::string valid = "valid";
	return m_interLeadEntry->GetId() == valid
		&& m_followDistEntry->GetId() == valid
		&& m_approachDistEntry->GetId() == valid
		&& m_strafeDistEntry->GetId() == valid
		&& m_sensorRangeEntry->GetId() == valid
		&& m_forwardThrustEntry->GetId() == valid
		&& m_sideThrustEntry->GetId() == valid
		&& m_reverseThrustEntry->GetId() == valid
		&& m_steerThrustEntry->GetId() == valid
		&& m_hullStrengthEntry->GetId() == valid
		&& m_frontShieldEntry->GetId() == valid
		&& m_sideShieldEntry->GetId() == valid
		&& m_rearShieldEntry->GetId() == valid
		&& m_shieldRegenEntry->GetId() == valid;
}

void ShipEditorWindow::CreateNewShip()
{
	if (m_newShipImageName.empty() || m_shipName.empty())
		return;

	ShipStats newShip = ShipStats();
	newShip.imageLocation = m_newShipImageName;

	Serializer<> ser;
	ser.Save(&newShip, m_shipName, m_shipName);

	OnShipSelected(m_shipName);
}

void ShipEditorWindow::ClearShipEditing() 
{
	m_shipImage = sf::Sprite();
	m_editingStats = nullptr;
	m_targetStats = nullptr;
	
	m_interLeadEntry->SetText("");
	m_followDistEntry->SetText("");
	m_approachDistEntry->SetText("");
	m_strafeDistEntry->SetText("");
	m_sensorRangeEntry->SetText("");
	m_forwardThrustEntry->SetText("");
	m_sideThrustEntry->SetText("");
	m_reverseThrustEntry->SetText("");
	m_steerThrustEntry->SetText("");
	m_hullStrengthEntry->SetText("");
	m_frontShieldEntry->SetText("");
	m_sideShieldEntry->SetText("");
	m_rearShieldEntry->SetText("");
	m_shieldRegenEntry->SetText("");

	DrawShipCanvas();
}

void ShipEditorWindow::DrawShipCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);
	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}

void ShipEditorWindow::OnShipSelected(const std::string& name)
{
	if (name.empty())
		return;

	m_shipName = name;

	m_targetStats = LoadShip(name);

	// Clone the ships stats for our use
	m_editingStats = std::make_unique<ShipStats>(*ShipStats::Clone(m_targetStats.get()));
	LoadShipStatsToEntries();
	LoadShipImage();

	DrawShipCanvas();
}

void ShipEditorWindow::OnNewShipImageSelected(const std::string& imageName)
{
	if (imageName.empty())
		return;

	m_newShipImageName = imageName;

	auto selectWindow = GetWindow<ShipNameEntry>("ship_name_entry");
	selectWindow->SetCallback([this](const std::string& name) { OnNewShipNameSelected(name); });
	selectWindow->Show(true);
}

void ShipEditorWindow::OnNewShipNameSelected(const std::string& name)
{
	if (name.empty())
		return;

	m_shipName = name;
	CreateNewShip();
}

void ShipEditorWindow::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	entry->SetId(WidgetHelpers::ValidateFloatEntry(entry)
		? "valid"
		: "invalid");
}

void ShipEditorWindow::OnSaveShip()
{
	// If any entries are invalid, prevent saving
	if (!CheckAllEntryValidity())
		return;

	m_editingStats->interceptLeadMultiplier = stof(m_interLeadEntry->GetText().toAnsiString());
	m_editingStats->followDistance = stof(m_followDistEntry->GetText().toAnsiString());
	m_editingStats->approachDistance = stof(m_approachDistEntry->GetText().toAnsiString());
	m_editingStats->strafeDistance = stof(m_strafeDistEntry->GetText().toAnsiString());
	m_editingStats->sensorRange = stof(m_sensorRangeEntry->GetText().toAnsiString());
	m_editingStats->maxHull = stof(m_hullStrengthEntry->GetText().toAnsiString());

	auto shipThrust = m_editingStats->GetShipThrust();
	shipThrust->Forward = stof(m_forwardThrustEntry->GetText().toAnsiString());
	shipThrust->Side = stof(m_sideThrustEntry->GetText().toAnsiString());
	shipThrust->Reverse = stof(m_reverseThrustEntry->GetText().toAnsiString());
	shipThrust->Steer = stof(m_steerThrustEntry->GetText().toAnsiString());

	auto shieldData = m_editingStats->GetShieldData();
	shieldData->FrontStrength = stof(m_frontShieldEntry->GetText().toAnsiString());
	shieldData->SideStrength = stof(m_sideShieldEntry->GetText().toAnsiString());
	shieldData->RearStrength = stof(m_rearShieldEntry->GetText().toAnsiString());
	shieldData->RegenSpeed = stof(m_shieldRegenEntry->GetText().toAnsiString());

	m_targetStats->Copy(m_editingStats.get());
	
	Serializer<> ser;
	ser.Save(m_targetStats.get(), m_shipName, m_shipName);
}

void ShipEditorWindow::OnDeleteShip() 
{
	assert(m_editingStats != nullptr);

	Serializer<> ser;
	ser.DeleteRecord(m_editingStats.get(), m_shipName);
	ClearShipEditing();
}