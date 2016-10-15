#include <UI/ShipEditorWindow.h>
#include "UI.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include "ShipSelector.h"

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
	SetupButtonSignals();

	m_shipWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	m_shipWindow->Add(m_shipCanvas);

	m_propertyTable = sfg::Table::Create();
	SetupPropertyTable();

	// Create layout widgets
	m_topLevelBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_leftSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_rightSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_leftSideBar->Pack(m_newShipButton);
	m_leftSideBar->Pack(m_editShipButton);
	m_leftSideBar->Pack(m_saveShipButton);

	m_rightSideBar->Pack(m_propertyTable);

	m_topLevelBox->Pack(m_leftSideBar);
	m_topLevelBox->Pack(m_shipWindow);
	m_topLevelBox->Pack(m_rightSideBar);
	m_window->Add(m_topLevelBox);
}

void ShipEditorWindow::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
		return;

	m_window->Show(val);

	if (m_containsMouse && !val)
		OnMouseLeave();
}

bool ShipEditorWindow::IsShown()
{
	return m_window->IsLocallyVisible();
}

void ShipEditorWindow::SetupWindowSignals()
{
	m_window->GetSignal(sfg::Window::OnMouseEnter).Connect([this] { OnMouseEnter(); });
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect([this] { OnMouseLeave(); });
	m_window->GetSignal(sfg::Window::OnCloseButton).Connect([this] { Show(false); });
}

void ShipEditorWindow::SetupButtonSignals()
{
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
		}
	);
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

	m_interLeadEntry->SetRequisition(sf::Vector2f(50, 0));
	m_followDistEntry->SetRequisition(sf::Vector2f(50, 0));
	m_approachDistEntry->SetRequisition(sf::Vector2f(50, 0));
	m_strafeDistEntry->SetRequisition(sf::Vector2f(50, 0));
	m_sensorRangeEntry->SetRequisition(sf::Vector2f(50, 0));

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

	m_forwardThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	m_sideThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	m_reverseThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	m_steerThrustEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(thrustHeaderLabel,	  { 0, 6,  2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(forwardThrustLabel,   { 0, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sideThrustLabel,	  { 0, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(reverseThrustLabel,	  { 0, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(steerThrustLabel,	  { 0, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_forwardThrustEntry, { 1, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_sideThrustEntry,	  { 1, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_reverseThrustEntry, { 1, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_steerThrustEntry,	  { 1, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto weaponsHeaderLabel = sfg::Label::Create("Weapon Stats");
	auto fireRateLabel = sfg::Label::Create("Fire Rate");
	auto heatLimitLabel = sfg::Label::Create("Heat Limit");
	auto cooldownRateLabel = sfg::Label::Create("Cooling Rate");
	auto heatGenLabel = sfg::Label::Create("Heat Generated");
	m_fireRateEntry = sfg::Entry::Create();
	m_heatLimitEntry = sfg::Entry::Create();
	m_cooldownRateEntry = sfg::Entry::Create();
	m_heatGenEntry = sfg::Entry::Create();

	m_fireRateEntry->SetRequisition(sf::Vector2f(50, 0));
	m_heatLimitEntry->SetRequisition(sf::Vector2f(50, 0));
	m_cooldownRateEntry->SetRequisition(sf::Vector2f(50, 0));
	m_heatGenEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(weaponsHeaderLabel, { 0, 11, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(fireRateLabel,		{ 0, 12, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatLimitLabel,		{ 0, 13, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(cooldownRateLabel,	{ 0, 14, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatGenLabel,		{ 0, 15, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_fireRateEntry,	{ 1, 12, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_heatLimitEntry,	{ 1, 13, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_cooldownRateEntry,{ 1, 14, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_heatGenEntry,		{ 1, 15, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
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
	m_fireRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_fireRateEntry); });
	m_heatLimitEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_heatLimitEntry); });
	m_cooldownRateEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_cooldownRateEntry); });
	m_heatGenEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_heatGenEntry); });
}

void ShipEditorWindow::LoadShipStatsToEntries()
{
	m_interLeadEntry->SetText(std::to_string(m_targetStats->GetInterceptLeadMultiplier()));
	m_followDistEntry->SetText(std::to_string(m_targetStats->GetFollowDistance()));
	m_approachDistEntry->SetText(std::to_string(m_targetStats->GetApproachDistance()));
	m_strafeDistEntry->SetText(std::to_string(m_targetStats->GetStrafeDistance()));
	m_sensorRangeEntry->SetText(std::to_string(m_targetStats->GetSensorRange()));
	m_forwardThrustEntry->SetText(std::to_string(m_targetStats->GetShipThrust()->Forward));
	m_sideThrustEntry->SetText(std::to_string(m_targetStats->GetShipThrust()->Side));
	m_reverseThrustEntry->SetText(std::to_string(m_targetStats->GetShipThrust()->Reverse));
	m_steerThrustEntry->SetText(std::to_string(m_targetStats->GetShipThrust()->Steer));
	m_fireRateEntry->SetText(std::to_string(m_targetStats->GetDirGunData()->fireRate));
	m_heatLimitEntry->SetText(std::to_string(m_targetStats->GetDirGunData()->heatLimit));
	m_cooldownRateEntry->SetText(std::to_string(m_targetStats->GetDirGunData()->cooldownRate));
	m_heatGenEntry->SetText(std::to_string(m_targetStats->GetDirGunData()->heatGenerated));
}

void ShipEditorWindow::LoadShipImage()
{
	m_shipTexture = LoadTexture(m_editingStats->GetImageLocation());
	m_shipImage.setTexture(*m_shipTexture.get());

	auto canvasSize = m_shipCanvas->GetRequisition();
	auto shipSize = sf::Vector2f(m_shipImage.getLocalBounds().width, m_shipImage.getLocalBounds().height);

	m_shipImage.setPosition((canvasSize / 2.f) - (shipSize / 2.f));

	DrawShipCanvas();
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
		&& m_fireRateEntry->GetId() == valid
		&& m_heatLimitEntry->GetId() == valid
		&& m_cooldownRateEntry->GetId() == valid
		&& m_heatGenEntry->GetId() == valid;
}

void ShipEditorWindow::DrawShipCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);
	m_shipCanvas->Unbind();
}

void ShipEditorWindow::OnShipSelected(const std::string& name)
{
	if (name == "")
		return;

	m_originalName = name;

	m_targetStats = LoadShip(name);

	// Clone the ships stats for our use
	m_editingStats = std::make_unique<ShipStats>(*ShipStats::Clone(m_targetStats.get()));
	LoadShipStatsToEntries();
	LoadShipImage();
}

void ShipEditorWindow::OnEntryFloatTextValidation(sfg::Entry::Ptr entry)
{
	std::string text = entry->GetText();
	
	// if text is empty, it's automatically invalid
	if (text.empty())
	{
		entry->SetId("invalid");
		return;
	}

	try
	{
		char* error = nullptr;
		float val = strtof(text.c_str(), &error);
		if (error[0] != '\0')
		{
			entry->SetId("invalid");
			return;
		}
		entry->SetId("valid");
	}
	catch (std::invalid_argument)
	{
		entry->SetId("invalid");
	}
}

void ShipEditorWindow::OnSaveShip()
{
	// If any entries are invalid, prevent saving
	if (!CheckAllEntryValidity())
		return;

	m_editingStats->SetInterceptLeadMultiplier(stof(m_interLeadEntry->GetText().toAnsiString()));
	m_editingStats->SetFollowDistance(stof(m_followDistEntry->GetText().toAnsiString()));
	m_editingStats->SetApproachDistance(stof(m_approachDistEntry->GetText().toAnsiString()));
	m_editingStats->SetStrafeDistance(stof(m_strafeDistEntry->GetText().toAnsiString()));
	m_editingStats->SetSensorRange(stof(m_sensorRangeEntry->GetText().toAnsiString()));

	auto shipThrust = m_editingStats->GetShipThrust();
	shipThrust->Forward = stof(m_forwardThrustEntry->GetText().toAnsiString());
	shipThrust->Side = stof(m_sideThrustEntry->GetText().toAnsiString());
	shipThrust->Reverse = stof(m_reverseThrustEntry->GetText().toAnsiString());
	shipThrust->Steer = stof(m_steerThrustEntry->GetText().toAnsiString());

	auto gunData = m_editingStats->GetDirGunData();
	gunData->fireRate = stof(m_fireRateEntry->GetText().toAnsiString());
	gunData->heatLimit = stof(m_heatLimitEntry->GetText().toAnsiString());
	gunData->cooldownRate = stof(m_cooldownRateEntry->GetText().toAnsiString());
	gunData->heatGenerated = stof(m_heatGenEntry->GetText().toAnsiString());

	m_targetStats->Copy(m_editingStats.get());
	
	serializer.Save(m_targetStats.get(), m_originalName, m_originalName);
}