// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <UI/ThrusterLocationEditor.h>
#include "UI.h"
#include "WidgetHelpers.h"
#include <ShipStats.h>
#include <Serializer.h>

ThrusterLocationEditor::ThrusterLocationEditor()
	: GameWindow("thruster_editor"),
	  m_shipTexture(nullptr)
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Thruster Editor");
	m_window->SetId("thruster_editor");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	table->Attach(m_shipCanvas, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	SetupCanvasSignals();

	auto resetButton = sfg::Button::Create("Reset");
	table->Attach(resetButton, { 0, 1, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	resetButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnReset(); });

	auto saveButton = sfg::Button::Create("Save and Exit");
	table->Attach(saveButton, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	saveButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnSave(); });
	auto closebutton = sfg::Button::Create("Close Without Saving");
	table->Attach(closebutton, { 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	closebutton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnClose(); });

	m_thrusterDisplayShape = sf::ConvexShape(5);
	m_thrusterDisplayShape.setPoint(0, { 0, -10 });
	m_thrusterDisplayShape.setPoint(1, { 0, 10 });
	m_thrusterDisplayShape.setPoint(2, { -10, 10 });
	m_thrusterDisplayShape.setPoint(3, { -15, 0 });
	m_thrusterDisplayShape.setPoint(4, { -10, -10 });
	m_thrusterDisplayShape.setFillColor(sf::Color::Red);
}

void ThrusterLocationEditor::SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats)
{
	m_targetStats = stats;
	m_shipName = shipName;

	LoadShipImage();

	auto& verts = m_targetStats->GetThrusterLocations();
	for (auto& v : verts)
	{
		AddThruster(v + m_shipImage.getPosition());
	}

	DrawCanvas();
}

void ThrusterLocationEditor::SetupCanvasSignals()
{
	m_shipCanvas->GetSignal(sfg::Canvas::OnLeftClick).Connect([this] { OnCanvasLeftClick(); });
}

void ThrusterLocationEditor::LoadShipImage()
{
	m_shipTexture = LoadTexture(m_targetStats->imageLocation);
	m_shipImage.setTexture(*m_shipTexture.get());
	// Explicitly set the texture rect. Otherwise, changing ships can cause part of
	// the image to be occluded
	m_shipImage.setTextureRect({ 0, 0, static_cast<int>(m_shipTexture->getSize().x), static_cast<int>(m_shipTexture->getSize().y) });

	auto canvasSize = m_shipCanvas->GetRequisition();
	auto shipSize = sf::Vector2f(m_shipImage.getLocalBounds().width, m_shipImage.getLocalBounds().height);

	m_shipImage.setPosition((canvasSize / 2.f) - (shipSize / 2.f));
}

void ThrusterLocationEditor::DrawCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);

	for (auto& v : m_locations)
	{
		m_thrusterDisplayShape.setPosition(v);
		m_shipCanvas->Draw(m_thrusterDisplayShape);
	}

	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}

void ThrusterLocationEditor::OnCanvasLeftClick()
{
	AddThruster(WidgetHelpers::GetRelativeMousePos(m_shipCanvas));

	DrawCanvas();
}

void ThrusterLocationEditor::AddThruster(const sf::Vector2f& pos)
{
	m_locations.push_back(pos);
}

void ThrusterLocationEditor::OnReset()
{
	m_locations.clear();
	DrawCanvas();
}

void ThrusterLocationEditor::OnSave()
{
	auto thrusterLocs = &m_targetStats->GetThrusterLocations();
	thrusterLocs->clear();
	auto shipPos = m_shipImage.getPosition();
	for (auto& v : m_locations)
	{
		thrusterLocs->push_back(v - shipPos);
	}

	Serializer<> ser;
	ser.Save(m_targetStats.get(), m_shipName, m_shipName);
	m_locations.clear();
	Show(false);
}

void ThrusterLocationEditor::OnClose()
{
	m_locations.clear();
	Show(false);
}
