#include <UI/InventoryWindow.h>
#include <EntityManager.h>
#include "UI.h"
#include <Components/ShipStatsSink.h>
#include <SFGUI/Box.hpp>
#include <Components/Health.h>
#include <Components/Inventory.h>

InventoryWindow::InventoryWindow()
	: GameWindow("inventory"), m_invenWidget(false)
{
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Inventory");
	m_window->SetId("inventory");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto mainBar = sfg::Box::Create(sfg::Box::Orientation::HORIZONTAL);
	auto leftSideBar = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
	
	// add a window to create a border around the canvas
	auto shipWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(250, 150));
	shipWindow->Add(m_shipCanvas);

	m_hullStateLabel = sfg::Label::Create("Hull State: 0%");
	m_creditsLabel = sfg::Label::Create("Credits: 0");

	leftSideBar->Pack(shipWindow);
	leftSideBar->Pack(m_hullStateLabel);
	leftSideBar->Pack(m_creditsLabel);

	m_invenWidget.SetSize({5, 5});

	mainBar->Pack(leftSideBar);
	mainBar->Pack(m_invenWidget.GetWidget());

	m_window->Add(mainBar);
}

void InventoryWindow::SetTarget(EntityID id)
{
	m_targetHandle = EntityManager::Get(id);

	LoadShipImage();
	LoadHullState();
	LoadCredits();
	DrawShipCanvas();

	m_invenWidget.SetTarget(id);
}

void InventoryWindow::LoadShipImage()
{
	auto stats = m_targetHandle->GetComponent<ShipStatsSink>();
	m_shipTexture = LoadTexture(stats->GetImageLocation());
	m_shipImage.setTexture(*m_shipTexture.get());
	// Explicitly set the texture rect. Otherwise, changing ships can cause part of
	// the image to be occluded
	m_shipImage.setTextureRect({ 0, 0, static_cast<int>(m_shipTexture->getSize().x), static_cast<int>(m_shipTexture->getSize().y) });

	auto canvasSize = m_shipCanvas->GetRequisition();
	auto shipSize = sf::Vector2f(m_shipImage.getLocalBounds().width, m_shipImage.getLocalBounds().height);

	m_shipImage.setPosition((canvasSize / 2.f) - (shipSize / 2.f));
}

void InventoryWindow::LoadHullState()
{
	auto health = m_targetHandle->GetComponent<Health>();
	float curHealth = health.GetHealthNormalized() * 100;

	std::stringstream ss;
	ss << std::fixed << "Hull State: " << std::setprecision(2) << curHealth << "%";

	m_hullStateLabel->SetText(ss.str());
}

void InventoryWindow::LoadCredits()
{
	auto inven = m_targetHandle->GetComponent<Inventory>();
	int curCredits = inven.GetCredits();

	std::stringstream ss;
	ss << "Credits: " << curCredits << '$';

	m_creditsLabel->SetText(ss.str());
}

void InventoryWindow::DrawShipCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);
	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}