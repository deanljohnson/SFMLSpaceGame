#include <UI/ShipEditorWindow.h>
#include "UI.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Entry.hpp>
#include <FileSystem.h>


ShipEditorWindow::ShipEditorWindow()
	: GameWindow("ship_editor")
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Ship Editor");
	m_window->SetId("ship_editor");
	UI::Singleton->Add(m_window);

	// Create widgets
	m_newShipButton = sfg::Button::Create("New Ship");
	m_editShipButton = sfg::Button::Create("Edit Ship");

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

	m_rightSideBar->Pack(m_propertyTable);

	m_topLevelBox->Pack(m_leftSideBar);
	m_topLevelBox->Pack(m_shipWindow);
	m_topLevelBox->Pack(m_rightSideBar);
	m_window->Add(m_topLevelBox);
	
	SetupWindowSignals();
}

void ShipEditorWindow::Show(bool val)
{
	if (m_window->IsLocallyVisible() == val)
		return;

	m_window->Show(val);
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

void ShipEditorWindow::SetupPropertyTable()
{
	auto aiHeaderLabel = sfg::Label::Create("AI Details");
	auto interLeadMultLabel = sfg::Label::Create("Intercept Lead Multiplier");
	auto followDistLabel = sfg::Label::Create("Follow Distance");
	auto approachDistLabel = sfg::Label::Create("Approach Distance");
	auto strafeDistLabel = sfg::Label::Create("Strafe Distance");
	auto sensorRangeLabel = sfg::Label::Create("Sensor Range");
	auto interLeadEntry = sfg::Entry::Create();
	auto followDistEntry = sfg::Entry::Create();
	auto approachDistEntry = sfg::Entry::Create();
	auto strafeDistEntry = sfg::Entry::Create();
	auto sensorRangeEntry = sfg::Entry::Create();

	interLeadEntry->SetRequisition(sf::Vector2f(50, 0));
	followDistEntry->SetRequisition(sf::Vector2f(50, 0));
	approachDistEntry->SetRequisition(sf::Vector2f(50, 0));
	strafeDistEntry->SetRequisition(sf::Vector2f(50, 0));
	sensorRangeEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(aiHeaderLabel,		{ 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(interLeadMultLabel, { 0, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(followDistLabel,	{ 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(approachDistLabel,	{ 0, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(strafeDistLabel,	{ 0, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sensorRangeLabel,	{ 0, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(interLeadEntry,		{ 1, 1, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(followDistEntry,	{ 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(approachDistEntry,	{ 1, 3, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(strafeDistEntry,	{ 1, 4, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sensorRangeEntry,	{ 1, 5, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto thrustHeaderLabel = sfg::Label::Create("Thrust Options");
	auto forwardThrustLabel = sfg::Label::Create("Forward");
	auto sideThrustLabel = sfg::Label::Create("Sideways");
	auto reverseThrustLabel = sfg::Label::Create("Reverse");
	auto steerThrustLabel = sfg::Label::Create("Steering");
	auto forwardThrustEntry = sfg::Entry::Create();
	auto sideThrustEntry = sfg::Entry::Create();
	auto reverseThrustEntry = sfg::Entry::Create();
	auto steerThrustEntry = sfg::Entry::Create();

	forwardThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	sideThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	reverseThrustEntry->SetRequisition(sf::Vector2f(50, 0));
	steerThrustEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(thrustHeaderLabel,	{ 0, 6,  2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(forwardThrustLabel, { 0, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sideThrustLabel,	{ 0, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(reverseThrustLabel, { 0, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(steerThrustLabel,	{ 0, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(forwardThrustEntry, { 1, 7,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(sideThrustEntry,	{ 1, 8,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(reverseThrustEntry, { 1, 9,  1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(steerThrustEntry,	{ 1, 10, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	auto weaponsHeaderLabel = sfg::Label::Create("Weapon Stats");
	auto fireRateLabel = sfg::Label::Create("Fire Rate");
	auto heatLimitLabel = sfg::Label::Create("Heat Limit");
	auto cooldownRateLabel = sfg::Label::Create("Cooling Rate");
	auto heatGenLabel = sfg::Label::Create("Heat Generated");
	auto fireRateEntry = sfg::Entry::Create();
	auto heatLimitEntry = sfg::Entry::Create();
	auto cooldownRateEntry = sfg::Entry::Create();
	auto heatGenEntry = sfg::Entry::Create();

	fireRateEntry->SetRequisition(sf::Vector2f(50, 0));
	heatLimitEntry->SetRequisition(sf::Vector2f(50, 0));
	cooldownRateEntry->SetRequisition(sf::Vector2f(50, 0));
	heatGenEntry->SetRequisition(sf::Vector2f(50, 0));

	m_propertyTable->Attach(weaponsHeaderLabel, { 0, 11, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(fireRateLabel,		{ 0, 12, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatLimitLabel,		{ 0, 13, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(cooldownRateLabel,	{ 0, 14, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatGenLabel,		{ 0, 15, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(fireRateEntry,		{ 1, 12, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatLimitEntry,		{ 1, 13, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(cooldownRateEntry,	{ 1, 14, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	m_propertyTable->Attach(heatGenEntry,		{ 1, 15, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);

	std::vector<std::string> results{};
	FileSystem::GetFileNamesWithExtension("..\\Data\\", ".ship", results);
	for (auto e : results)
	{
		printf("%s\n", e.c_str());
	}
}
