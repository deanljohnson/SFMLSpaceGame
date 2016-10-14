#include <Components/Text.h>
#include <UI/UI.h>

Text::Text(const std::string& text)
{
	m_label = sfg::Label::Create(text);
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