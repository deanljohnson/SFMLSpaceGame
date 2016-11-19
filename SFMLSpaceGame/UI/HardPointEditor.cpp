#include "stdafx.h"
#include <UI/HardPointEditor.h>
#include "UI.h"
#include <VectorMath.h>
#include "WidgetHelpers.h"

HardPointEditor::HardPointEditor()
	: GameWindow("hard_point_editor"),
	  m_shipTexture(nullptr),
	  m_hardPointType(HardPointWidget::Type::Gun)
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
	table->Attach(m_shipCanvas, {1, 0, 2, 4}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	SetupCanvasSignals();

	auto gunRadio = sfg::RadioButton::Create("Place Gun");
	gunRadio->SetActive(true);
	auto launcherRadio = sfg::RadioButton::Create("Place Launcher", gunRadio->GetGroup());
	table->Attach(gunRadio, {0, 1, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(launcherRadio, {0, 2, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	gunRadio->GetSignal(sfg::RadioButton::OnToggle).Connect([this, gunRadio] 
	{ 
		if (!gunRadio->IsActive()) return;
		OnSelectHardPointType(HardPointWidget::Type::Gun); 
	});
	launcherRadio->GetSignal(sfg::RadioButton::OnToggle).Connect([this, launcherRadio]
	{ 
		if (!launcherRadio->IsActive()) return;
		OnSelectHardPointType(HardPointWidget::Type::MissileLauncher); 
	});

	auto deleteButton = sfg::Button::Create("Delete Selected Hardpoint");
	table->Attach(deleteButton, { 1, 5, 2, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	deleteButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnDeleteSelectHardpoint(); });

	auto saveButton = sfg::Button::Create("Save and Exit");
	table->Attach(saveButton, { 1, 6, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	saveButton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnSave(); });
	auto closebutton = sfg::Button::Create("Close Without Saving");
	table->Attach(closebutton, { 2, 6, 1, 1 }, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	closebutton->GetSignal(sfg::Button::OnLeftClick).Connect([this] { OnClose(); });
}

void HardPointEditor::SetTarget(const std::string& shipName, std::shared_ptr<ShipStats> stats)
{
	m_targetStats = stats;
	m_shipName = shipName;

	LoadShipImage();

	m_hardPointWidgets.clear();
	auto& gunHardPoints = m_targetStats->GetDirGunData()->hardPoints;
	for (auto& hp : gunHardPoints)
	{
		auto pos = B2VecToSFMLVec(hp.positionOffset) + m_shipImage.getPosition();
		m_hardPointWidgets.push_back(HardPointWidget(pos, HardPointWidget::Type::Gun));
		m_hardPointWidgets.back().SetAngle(hp.angleOffset);
	}

	auto& missileHardPoints = m_targetStats->GetMissileLauncherData()->hardPoints;
	for (auto& hp : missileHardPoints) 
	{
		auto pos = B2VecToSFMLVec(hp.positionOffset) + m_shipImage.getPosition();
		m_hardPointWidgets.push_back(HardPointWidget(pos, HardPointWidget::Type::MissileLauncher));
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

void HardPointEditor::CreateHardpoint(const sf::Vector2f& pos)
{
	m_hardPointWidgets.push_back(HardPointWidget(pos, m_hardPointType));

	DrawCanvas();
}

void HardPointEditor::OnCanvasLeftClick()
{
	if (m_dragOnCanvas)
		return;

	auto inCanvasPos = WidgetHelpers::GetRelativeMousePos(m_shipCanvas);

	for (int i = 0; i < m_hardPointWidgets.size(); i++)
	{
		if (m_hardPointWidgets[i].PositionControlContains(inCanvasPos.x, inCanvasPos.y)
			|| m_hardPointWidgets[i].AngleControlContains(inCanvasPos.x, inCanvasPos.y))
		{
			// unselect any currently selected widget
			if (m_lastClickedWidget != -1 && m_lastClickedWidget < m_hardPointWidgets.size())
				m_hardPointWidgets[m_lastClickedWidget].UnSelect();

			// select the clicked widget
			m_lastClickedWidget = i;
			m_hardPointWidgets[i].Select();

			// Draw the canvas to show the selection
			DrawCanvas();
			return;
		}
	}

	// No existing widget was selected, create a new one
	CreateHardpoint(inCanvasPos);
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
		auto inCanvasPos = WidgetHelpers::GetRelativeMousePos(m_shipCanvas);

		if (m_mouseDownOnPositionWidget)
			m_hardPointWidgets[m_mouseDownOnWidget].SetPositionControlLocation(inCanvasPos.x, inCanvasPos.y);
		else if (m_mouseDownOnAngleWidget)
			m_hardPointWidgets[m_mouseDownOnWidget].SetAngleControlLocation(inCanvasPos.x, inCanvasPos.y);

		DrawCanvas();
	}
}

void HardPointEditor::OnCanvasLeftPress()
{
	auto inCanvasPos = WidgetHelpers::GetRelativeMousePos(m_shipCanvas);

	for (int i = 0; i < m_hardPointWidgets.size(); i++)
	{
		m_mouseDownOnPositionWidget = m_hardPointWidgets[i].PositionControlContains(inCanvasPos.x, inCanvasPos.y);
		m_mouseDownOnAngleWidget = m_hardPointWidgets[i].AngleControlContains(inCanvasPos.x, inCanvasPos.y);
		
		// If the click was on a control widget
		if (m_mouseDownOnPositionWidget
			|| m_mouseDownOnAngleWidget)
		{
			// track which widget we clicked on
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

void HardPointEditor::OnSelectHardPointType(HardPointWidget::Type type) 
{
	m_hardPointType = type;

	switch (m_hardPointType)
	{
	case HardPointWidget::Type::Gun:
		printf("gun\n");
		break;
	case HardPointWidget::Type::MissileLauncher:
		printf("missile\n");
		break;
	}
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
	auto missileData = m_targetStats->GetMissileLauncherData();

	// Clear the previous data
	gunData->hardPoints.clear();
	missileData->hardPoints.clear();

	// Transfer the editor data to the data objects
	for (auto& hp : m_hardPointWidgets)
	{
		auto pos = hp.GetHardPointLocation();
		pos -= m_shipImage.getPosition();

		switch (hp.GetType()) 
		{
		case HardPointWidget::Type::Gun:
			gunData->hardPoints.push_back(HardPoint({ pos.x, pos.y }, hp.GetAngle()));
			break;
		case HardPointWidget::Type::MissileLauncher:
			missileData->hardPoints.push_back(HardPoint({ pos.x, pos.y }, hp.GetAngle()));
			break;
		}
	}

	// Save the data to disk
	serializer.Save(m_targetStats.get(), m_shipName, m_shipName);

	// Clear our editor widgets
	m_hardPointWidgets.clear();
	Show(false);
}

void HardPointEditor::OnClose()
{
	m_hardPointWidgets.clear();
	Show(false);
}