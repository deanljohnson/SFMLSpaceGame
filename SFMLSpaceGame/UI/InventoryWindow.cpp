#include "stdafx.h"
#include <UI/InventoryWindow.h>
#include <EntityManager.h>
#include <UI/UI.h>
#include <UI/ItemContextProvider.h>
#include <PlayerData.h>
#include <Components/Health.h>
#include <Components/Inventory.h>
#include <Equipper.h>
#include <Components/ShipStatsComponent.h>

InventoryWindow::InventoryWindow()
	: GameWindow("inventory"),
	  m_invenWidget(),
	  m_contextProvider(std::make_shared<ItemContextProvider>())
{
	m_invenWidget.SetContextProvider(m_contextProvider);
	m_contextProvider->SetEquipHandler(
		[this](std::shared_ptr<Item> i, size_t slot)
		{
			RemoveItem(i);
			Equipper::Equip(i, m_targetHandle.GetID(), slot);
		});
	m_contextProvider->SetHoverHandler(
		[this](std::shared_ptr<Item> i, size_t slot, bool highlight)
		{
			HardPointWidget::Type t;
			switch (i->type)
			{
			case ItemType::LaserRig:
				t = HardPointWidget::Type::Gun;
				break;
			default:
				throw "unrecognized item type for hover handler";
			}

			HighlightHardPoint(t, slot, highlight);
		});

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
	LoadHardPoints();
	DrawShipCanvas();

	m_invenWidget.SetTarget(id);
	m_contextProvider->SetEquipSlotCounts(ItemType::LaserRig, 
										Equipper::GetNumSlots(ItemType::LaserRig, id));
}

void InventoryWindow::LoadShipImage()
{
	auto stats = LoadShip(PlayerData::GetActive()->GetPlayerShip());
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

void InventoryWindow::LoadHardPoints()
{
	auto& targetStats = m_targetHandle->GetComponent<ShipStatsComponent>();

	m_hardPointWidgets.clear();
	auto& gunHardPoints = targetStats->GetDirGunData()->hardPoints;
	for (auto& hp : gunHardPoints)
	{
		auto pos = B2VecToSFMLVec(hp.positionOffset) + m_shipImage.getPosition();
		m_hardPointWidgets.push_back(HardPointWidget(pos, HardPointWidget::Type::Gun));
		m_hardPointWidgets.back().SetAngle(hp.angleOffset);
		m_hardPointWidgets.back().SetAlpha(100); // start dim
	}

	auto& missileHardPoints = targetStats->GetMissileLauncherData()->hardPoints;
	for (auto& hp : missileHardPoints)
	{
		auto pos = B2VecToSFMLVec(hp.positionOffset) + m_shipImage.getPosition();
		m_hardPointWidgets.push_back(HardPointWidget(pos, HardPointWidget::Type::MissileLauncher));
		m_hardPointWidgets.back().SetAngle(hp.angleOffset);
		m_hardPointWidgets.back().SetAlpha(100); // start dim
	}
}

void InventoryWindow::DrawShipCanvas()
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

void InventoryWindow::RemoveItem(std::shared_ptr<Item> item)
{
	auto& inven = m_targetHandle->GetComponent<Inventory>();
	inven.RemoveItem(item);

	// This rebuilds the inventory in the UI
	SetTarget(m_targetHandle.GetID());
}

void InventoryWindow::HighlightHardPoint(HardPointWidget::Type type, size_t slot, bool highlight)
{
	// all types of hard points are kept in the same list, 
	// so we need to iterate through them and count according
	// to the passed in type until we find the right one
	for (size_t i = 0; i < m_hardPointWidgets.size(); i++)
	{
		// found the right one
		if (slot == 0)
		{
			// change alpha to give a highlighting effect
			int8 alpha = highlight ? 255 : 100;
			m_hardPointWidgets[i].SetAlpha(alpha);
			DrawShipCanvas(); // redraw to see the change
			break;
		}
		if (m_hardPointWidgets[i].GetType() == type)
		{
			slot--;
		}
	}
}