#include <UI/HardPointEditor.h>
#include "UI.h"
#include <WorldConstants.h>
#include <SFGUI/Table.hpp>
#include <SFGUI/Button.hpp>
#include <VectorMath.h>

HardPointEditor::HardPointEditor()
	: GameWindow("hard_point_editor"),
	  m_shipTexture(nullptr)
{
	// Create main window
	m_window = sfg::Window::Create(sfg::Window::TOPLEVEL | sfg::Window::CLOSE);
	m_window->SetTitle("Hardpoint Editor");
	m_window->SetId("hard_point_editor");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	m_shipCanvas = sfg::Canvas::Create();
	m_shipCanvas->SetRequisition(sf::Vector2f(500, 300));
	table->Attach(m_shipCanvas, {0, 0, 2, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	SetupCanvasSignals();

	auto deleteButton = sfg::Button::Create("Delete Selected Hardpoint");
	table->Attach(deleteButton, { 0, 1, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	deleteButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnDeleteSelectHardpoint(); });

	auto saveButton = sfg::Button::Create("Save and Exit");
	table->Attach(saveButton, { 0, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	saveButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnSave(); });
	auto closebutton = sfg::Button::Create("Close Without Saving");
	table->Attach(closebutton, { 1, 2, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	closebutton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnClose(); });
}

void HardPointEditor::SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats)
{
	m_targetStats = stats;
	m_shipName = shipName;

	LoadShipImage();

	m_hardPointWidgets.clear();
	auto& hardPoints = m_targetStats->GetDirGunData()->hardPoints;
	for (auto& hp : hardPoints)
	{
		m_hardPointWidgets.push_back(HardPointWidget(B2VecToSFMLVec(hp.positionOffset) + m_shipImage.getPosition()));
		m_hardPointWidgets.back().SetAngle(hp.angleOffset);
	}

	DrawCanvas();
}

void HardPointEditor::SetupCanvasSignals()
{
	m_shipCanvas->GetSignal(sfg::Canvas::OnLeftClick).Connect([this] { OnCanvasLeftClick(); });
	m_shipCanvas->GetSignal(sfg::Canvas::OnMouseMove).Connect([this] { OnCanvasMouseMove(); });
	m_shipCanvas->GetSignal(sfg::Canvas::OnMouseLeftPress).Connect([this] { OnCanvasLeftPress(); });
	m_shipCanvas->GetSignal(sfg::Canvas::OnMouseLeftRelease).Connect([this] { OnCanvasLeftRelease(); });
	m_shipCanvas->GetSignal(sfg::Canvas::OnMouseLeave).Connect([this] { OnCanvasMouseLeave(); });
}

void HardPointEditor::LoadShipImage()
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

void HardPointEditor::DrawCanvas()
{
	m_shipCanvas->Bind();
	m_shipCanvas->Clear(sf::Color::Transparent);
	m_shipCanvas->Draw(m_shipImage);

	for (auto& h : m_hardPointWidgets)
	{
		m_shipCanvas->Draw(h);
	}

	m_shipCanvas->Display();
	m_shipCanvas->Unbind();
}

void HardPointEditor::OnCanvasLeftClick()
{
	if (m_dragOnCanvas)
		return;

	auto mousePos = sf::Mouse::getPosition(*GAME_WINDOW);
	auto canvasPos = m_shipCanvas->GetAbsolutePosition();
	auto inCanvasPos = sf::Vector2f(mousePos.x, mousePos.y) - canvasPos;

	for (int i = 0; i < m_hardPointWidgets.size(); i++)
	{
		if (m_hardPointWidgets[i].PositionControlContains(inCanvasPos.x, inCanvasPos.y)
			|| m_hardPointWidgets[i].AngleControlContains(inCanvasPos.x, inCanvasPos.y))
		{
			if (m_lastClickedWidget != -1 && m_lastClickedWidget < m_hardPointWidgets.size())
				m_hardPointWidgets[m_lastClickedWidget].UnSelect();

			m_lastClickedWidget = i;
			m_hardPointWidgets[i].Select();
			DrawCanvas();
			return;
		}
	}

	m_hardPointWidgets.push_back(HardPointWidget(inCanvasPos));

	DrawCanvas();
}

void HardPointEditor::OnCanvasMouseMove()
{
	if (!m_dragOnCanvas)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			m_dragOnCanvas = true;
		else return;
	}
	else if (m_mouseDownOnWidget != -1 && m_mouseDownOnWidget < m_hardPointWidgets.size())
	{
		auto mousePos = sf::Mouse::getPosition(*GAME_WINDOW);
		auto canvasPos = m_shipCanvas->GetAbsolutePosition();
		auto inCanvasPos = sf::Vector2f(mousePos.x, mousePos.y) - canvasPos;

		if (m_mouseDownOnPositionWidget)
			m_hardPointWidgets[m_mouseDownOnWidget].SetPositionControlLocation(inCanvasPos.x, inCanvasPos.y);
		else if (m_mouseDownOnAngleWidget)
			m_hardPointWidgets[m_mouseDownOnWidget].SetAngleControlLocation(inCanvasPos.x, inCanvasPos.y);

		DrawCanvas();
	}
}

void HardPointEditor::OnCanvasLeftPress()
{
	auto mousePos = sf::Mouse::getPosition(*GAME_WINDOW);
	auto canvasPos = m_shipCanvas->GetAbsolutePosition();
	auto inCanvasPos = sf::Vector2f(mousePos.x, mousePos.y) - canvasPos;

	for (int i = 0; i < m_hardPointWidgets.size(); i++)
	{
		if ((m_mouseDownOnPositionWidget = m_hardPointWidgets[i].PositionControlContains(inCanvasPos.x, inCanvasPos.y))
			|| (m_mouseDownOnAngleWidget = m_hardPointWidgets[i].AngleControlContains(inCanvasPos.x, inCanvasPos.y)))
		{
			m_mouseDownOnWidget = i;
			return;
		}
	}
}

void HardPointEditor::OnCanvasLeftRelease()
{
	m_dragOnCanvas = false;
	m_mouseDownOnWidget = -1;
}

void HardPointEditor::OnCanvasMouseLeave()
{
	m_dragOnCanvas = false;
}

void HardPointEditor::OnDeleteSelectHardpoint()
{
	if (m_lastClickedWidget != -1 && m_lastClickedWidget < m_hardPointWidgets.size())
	{
		m_hardPointWidgets.erase(m_hardPointWidgets.begin() + m_lastClickedWidget);
		m_lastClickedWidget = -1;
		DrawCanvas();
	}
}

void HardPointEditor::OnSave()
{
	auto gunData = m_targetStats->GetDirGunData();
	gunData->hardPoints.clear();

	for (auto& hp : m_hardPointWidgets)
	{
		auto pos = hp.GetHardPointLocation();
		pos -= m_shipImage.getPosition();

		gunData->hardPoints.push_back(HardPoint({ pos.x, pos.y }, hp.GetAngle()));
	}

	serializer.Save(m_targetStats.get(), m_shipName, m_shipName);

	m_hardPointWidgets.clear();
	Show(false);
}

void HardPointEditor::OnClose()
{
	m_hardPointWidgets.clear();
	Show(false);
}