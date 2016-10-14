#include <Components/Text.h>
#include <UI/UI.h>

Text::Text(const std::string& text)
{
	m_centerAlignment = sfg::Alignment::Create();
	UI::Singleton->Add(m_centerAlignment, true);

	m_label = sfg::Label::Create(text);
	m_centerAlignment->Add(m_label);
	m_label->SetPosition(sf::Vector2f(0, 100));
}

void Text::OnEnable()
{
	UI::Singleton->Add(m_centerAlignment, true);
	m_label->Show(true);
}

void Text::OnDisable()
{
	UI::Singleton->Remove(m_centerAlignment, true);
	m_label->Show(false);
}