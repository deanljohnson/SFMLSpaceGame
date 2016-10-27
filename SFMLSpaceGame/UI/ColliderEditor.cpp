#include "stdafx.h"
#include <UI/ColliderEditor.h>
#include "UI.h"
#include "WidgetHelpers.h"

ColliderEditor::ColliderEditor()
	: GameWindow("collider_editor"),
	  m_shipTexture(nullptr)
{
	m_verts.setPrimitiveType(sf::LineStrip);
	m_fillShape = sf::ConvexShape();
	m_fillShape.setPointCount(0);
	m_fillShape.setFillColor(sf::Color(255, 255, 0, 150));
	m_fillShape.setOutlineColor(sf::Color::Transparent);

	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Collider Editor");
	m_window->SetId("collider_editor");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	table->Attach(m_shipCanvas, { 0, 0, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	SetupCanvasSignals();

	auto resetButton = sfg::Button::Create("Reset");
	table->Attach(resetButton, {0, 1, 2, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	resetButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnReset(); });

	auto saveButton = sfg::Button::Create("Save and Exit");
	table->Attach(saveButton, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	saveButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnSave(); });
	auto closebutton = sfg::Button::Create("Close Without Saving");
	table->Attach(closebutton, { 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	closebutton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnClose(); });
}

void ColliderEditor::SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats)
{
	m_targetStats = stats;
	m_shipName = shipName;

	LoadShipImage();

	auto& verts = m_targetStats->GetColliderVertices();
	for (auto& v : verts)
	{
		AddVertex(v + m_shipImage.getPosition());
	}

	DrawCanvas();
}

void ColliderEditor::SetupCanvasSignals()
{
	m_shipCanvas->GetSignal(sfg::Canvas::OnLeftClick).Connect([this] { OnCanvasLeftClick(); });
}

void ColliderEditor::LoadShipImage()
{
	m_shipTexture = LoadTexture(m_targetStats->GetImageLocation());
	m_shipImage.setTexture(*m_shipTexture.get());
	// Explicitly set the texture rect. Otherwise, changing ships can cause part of
	// the image to be occluded
	m_shipImage.setTextureRect({ 0, 0, static_cast<int>(m_shipTexture->getSize().x), static_cast<int>(m_shipTexture->getSize().y) });

	auto canvasSize = m_shipCanvas->GetRequisition();
	auto shipSize = sf::Vector2f(m_shipImage.getLocalBounds().width, m_shipImage.getLocalBounds().height);

	m_shipImage.setPosition((canvasSize / 2.f) - (shipSize / 2.f));
}

void ColliderEditor::DrawCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);

	if (m_verts.getVertexCount() >= 3)
	{
		m_shipCanvas->Draw(m_fillShape);
	}

	m_shipCanvas->Draw(m_verts);

	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}

void ColliderEditor::OnCanvasLeftClick()
{
	AddVertex(WidgetHelpers::GetRelativeMousePos(m_shipCanvas));

	DrawCanvas();
}

void ColliderEditor::AddVertex(sf::Vector2f pos)
{
	m_verts.append(sf::Vertex(pos, sf::Color::White));
	m_fillShape.setPointCount(m_fillShape.getPointCount() + 1);
	m_fillShape.setPoint(m_fillShape.getPointCount() - 1, pos);
}

void ColliderEditor::OnReset()
{
	m_verts.clear();
	m_fillShape.setPointCount(0);
	DrawCanvas();
}

void ColliderEditor::OnSave()
{
	auto colliderVerts = &m_targetStats->GetColliderVertices();
	colliderVerts->clear();
	for (int i = 0; i < m_verts.getVertexCount(); i++)
	{
		auto& v = m_verts[i];
		colliderVerts->push_back(v.position - m_shipImage.getPosition());
	}

	m_serializer.Save(m_targetStats.get(), m_shipName, m_shipName);
	m_verts.clear();
	m_fillShape.setPointCount(0);
	Show(false);
}

void ColliderEditor::OnClose()
{
	m_verts.clear();
	m_fillShape.setPointCount(0);
	Show(false);
}
