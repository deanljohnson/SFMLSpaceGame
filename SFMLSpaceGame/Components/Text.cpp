// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <Components/Text.h>
#include <UI/UI.h>

Text::Text(EntityID ent, const std::string& text)
	: Component(ent),
	  m_label(sfg::Label::Create(text))
{
	m_label->SetPosition(sf::Vector2f((UI::Singleton->GetSize().x / 2.f) - m_label->GetAllocation().width / 2.f, UI::Singleton->GetSize().y - 300.f));
	UI::Singleton->Add(m_label);
}

void Text::OnEnable()
{
	UI::Singleton->Add(m_label);
	m_label->Show(true);
}

void Text::OnDisable()
{
	UI::Singleton->Remove(m_label);
	m_label->Show(false);
}