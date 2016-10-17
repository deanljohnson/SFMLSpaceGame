#include <UI/ShipEditorWindow.h>
#include "UI.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include "ShipSelector.h"
#include "ImageSelector.h"
#include "ShipNameEntry.h"
#include <WorldConstants.h>
#include "HardPointEditor.h"

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
	m_defineColliderButton = sfg::Button::Create("Begin Defining Collider");
	m_hardpointEditorButton = sfg::Button::Create("Edit Hardpoints");
	SetupButtonSignals();

	m_shipWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	m_shipWindow->Add(m_shipCanvas);
	SetupCanvasSignals();

	m_propertyTable = sfg::Table::Create();
	SetupPropertyTable();

	// Create layout widgets
	m_topLevelBox = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	m_mainBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_leftSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	m_rightSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_leftSideBar->Pack(m_newShipButton);
	m_leftSideBar->Pack(m_editShipButton);
	m_leftSideBar->Pack(m_saveShipButton);

	m_mainBox->Pack(m_shipWindow);
	m_mainBox->Pack(m_defineColliderButton);
	m_mainBox->Pack(m_hardpointEditorButton);

	m_rightSideBar->Pack(m_propertyTable);
	
	m_topLevelBox->Pack(m_leftSideBar);
	m_topLevelBox->Pack(m_mainBox);
	m_topLevelBox->Pack(m_rightSideBar);
	m_window->Add(m_topLevelBox);

	m_colliderVertices.setPrimitiveType(sf::LineStrip);
}

void ShipEditorWindow::SetPosition(const sf::Vector2f& pos)
{
	m_window->SetPosition(pos);
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
		}
	);

	m_defineColliderButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this] 
		{ 
			if (!m_definingCollider) BeginDefiningCollider();
			else EndDefiningCollider();
		}
	);

	m_hardpointEditorButton->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this]
		{
			if (m_targetStats.get() == nullptr)
				return;
			auto editWindow = static_cast<HardPointEditor*>(GetWindow("hard_point_editor"));
			editWindow->SetTarget(m_shipName, m_targetStats);
			editWindow->Show(true);
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

	auto defenseHeaderLabel = sfg::Label::Create("Defense Stats");
	auto hullStrengthLabel = sfg::Label::Create("Hull Strength");
	m_hullStrengthEntry = sfg::Entry::Create();

	m_hullStrengthEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(defenseHeaderLabel,		{ 0, 16, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(hullStrengthLabel,		{ 0, 17, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(m_hullStrengthEntry,	{ 1, 17, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	SetupEntryValidationSignals();
}

void ShipEditorWindow::SetupCanvasSignals()
{
	m_shipCanvas->GetSignal(sfg::Canvas::OnLeftClick).Connect([this] { OnCanvasClick(); });
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
	m_hullStrengthEntry->GetSignal(sfg::Entry::OnTextChanged).Connect(
		[this] { OnEntryFloatTextValidation(m_hullStrengthEntry); });
}

void ShipEditorWindow::BeginDefiningCollider()
{
	m_definingCollider = true;
	m_defineColliderButton->SetLabel("End Collider Definition");

	m_colliderVertices.clear();
}

void ShipEditorWindow::EndDefiningCollider()
{
	m_defineColliderButton->SetLabel("Begin Collider Definition");

	// Close the collider shape
	if (m_colliderVertices.getVertexCount() > 2)
		m_colliderVertices.append(m_colliderVertices[0]);

	DrawShipCanvas();
	m_definingCollider = false;
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
	m_hullStrengthEntry->SetText(std::to_string(m_targetStats->GetMaxHullStrength()));
}

void ShipEditorWindow::LoadShipImage()
{
	m_shipTexture = LoadTexture(m_editingStats->GetImageLocation());
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
		&& m_fireRateEntry->GetId() == valid
		&& m_heatLimitEntry->GetId() == valid
		&& m_cooldownRateEntry->GetId() == valid
		&& m_heatGenEntry->GetId() == valid
		&& m_hullStrengthEntry->GetId() == valid;
}

void ShipEditorWindow::CreateNewShip()
{
	if (m_newShipImageName == "" || m_shipName == "")
		return;

	ShipStats newShip = ShipStats();
	newShip.SetImageLocation(m_newShipImageName);
	serializer.Save(&newShip, m_shipName, m_shipName);

	OnShipSelected(m_shipName);
}

void ShipEditorWindow::DrawShipCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);

	m_shipCanvas->Draw(m_colliderVertices);

	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}

void ShipEditorWindow::OnCanvasClick()
{
	if (!m_definingCollider)
		return;

	auto mousePos = sf::Mouse::getPosition(*GAME_WINDOW);
	auto canvasPos = m_shipCanvas->GetAbsolutePosition();
	auto inCanvasPos = sf::Vector2f(mousePos.x, mousePos.y) - canvasPos;
	auto spriteRelativePos = inCanvasPos - m_shipImage.getPosition();

	if (spriteRelativePos.x < 0) spriteRelativePos.x = 0;
	if (spriteRelativePos.y < 0) spriteRelativePos.y = 0;
	if (spriteRelativePos.x > m_shipImage.getLocalBounds().width) 
		spriteRelativePos.x = m_shipImage.getLocalBounds().width;
	if (spriteRelativePos.y > m_shipImage.getLocalBounds().height)
		spriteRelativePos.y = m_shipImage.getLocalBounds().height;

	m_colliderVertices.append(sf::Vertex(spriteRelativePos + m_shipImage.getPosition(), sf::Color::White));
	DrawShipCanvas();
}

void ShipEditorWindow::OnShipSelected(const std::string& name)
{
	if (name == "")
		return;

	m_shipName = name;

	m_targetStats = LoadShip(name);

	// Clone the ships stats for our use
	m_editingStats = std::make_unique<ShipStats>(*ShipStats::Clone(m_targetStats.get()));
	LoadShipStatsToEntries();
	LoadShipImage();

	m_colliderVertices.clear();
	auto& verts = m_editingStats->GetColliderVertices();
	for (auto& v : verts)
	{
		m_colliderVertices.append(sf::Vertex(m_shipImage.getPosition() + v, sf::Color::White));
	}
	if (!verts.empty())
		m_colliderVertices.append(sf::Vertex(m_shipImage.getPosition() + verts[0], sf::Color::White));

	DrawShipCanvas();
}

void ShipEditorWindow::OnNewShipImageSelected(const std::string& imageName)
{
	if (imageName == "")
		return;

	m_newShipImageName = imageName;

	auto selectWindow = static_cast<ShipNameEntry*>(GetWindow("ship_name_entry"));
	selectWindow->SetCallback([this](const std::string& name) { OnNewShipNameSelected(name); });
	selectWindow->Show(true);
}

void ShipEditorWindow::OnNewShipNameSelected(const std::string& name)
{
	if (name == "")
		return;

	m_shipName = name;
	CreateNewShip();
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
		auto val = strtof(text.c_str(), &error);
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
	m_editingStats->SetMaxHullStrength(stof(m_hullStrengthEntry->GetText().toAnsiString()));

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

	auto colliderVerts = &m_editingStats->GetColliderVertices();
	colliderVerts->clear();
	for (int i = 0; i < m_colliderVertices.getVertexCount() - 1; i++)
	{
		auto& v = m_colliderVertices[i];
		colliderVerts->push_back(v.position - m_shipImage.getPosition());
	}

	m_targetStats->Copy(m_editingStats.get());
	
	serializer.Save(m_targetStats.get(), m_shipName, m_shipName);
}