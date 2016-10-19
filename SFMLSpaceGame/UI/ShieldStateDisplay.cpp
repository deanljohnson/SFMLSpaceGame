#include <UI/ShieldStateDisplay.h>
#include <EntityManager.h>
#include <UI/UI.h>
#include <SFGUI\Table.hpp>
#include <SFGUI\Label.hpp>

ShieldStateDisplay::ShieldStateDisplay() 
	: GameWindow("shield_display")
{
	m_window = sfg::Window::Create(sfg::Window::NO_STYLE);
	m_window->SetId("shield_display");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	auto table = sfg::Table::Create();
	m_window->Add(table);

	auto frontLabel = sfg::Label::Create("Front");
	auto sideLabel = sfg::Label::Create("Side");
	auto rearLabel = sfg::Label::Create("Rear");
	m_frontState = sfg::ProgressBar::Create();
	m_sideState = sfg::ProgressBar::Create();
	m_rearState = sfg::ProgressBar::Create();

	m_frontState->SetRequisition(sf::Vector2f(50, 0));
	m_sideState->SetRequisition(sf::Vector2f(50, 0));
	m_rearState->SetRequisition(sf::Vector2f(50, 0));

	table->Attach(frontLabel, {0, 0, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(sideLabel, {0, 1, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(rearLabel, {0, 2, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_frontState, {1, 0, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_sideState, {1, 1, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
	table->Attach(m_rearState, {1, 2, 1, 1}, sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL);
}

void ShieldStateDisplay::Update() 
{
	if (!m_targetHandle.IsValid())
		return;

	auto front = m_targetShields->GetCurrentStrength(Shields::Direction::Front) 
				/ m_targetShields->GetMaxStrength(Shields::Direction::Front);
	auto side = m_targetShields->GetCurrentStrength(Shields::Direction::Side)
		/ m_targetShields->GetMaxStrength(Shields::Direction::Side);
	auto rear = m_targetShields->GetCurrentStrength(Shields::Direction::Rear)
		/ m_targetShields->GetMaxStrength(Shields::Direction::Rear);
	m_frontState->SetFraction(front);
	m_sideState->SetFraction(side);
	m_rearState->SetFraction(rear);
}

void ShieldStateDisplay::SetTarget(const EntityID& target) 
{
	m_targetHandle = EntityManager::Get(target);
	assert(m_targetHandle->HasComponent<Shields>());

	m_targetShields = &m_targetHandle->GetComponent<Shields>();
}