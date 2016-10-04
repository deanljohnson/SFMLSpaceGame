#include <UI/UIText.h>

UIText::UIText(std::string text, ResourceID fontID)
{
	m_font = LoadFont(fontID);

	m_text.setFont(*m_font.get());
	m_text.setString(text);
	m_text.setColor(sf::Color::White);
}

void UIText::Refresh(std::string text, ResourceID fontID)
{
	m_text.setString(text);

	m_font = LoadFont(fontID);
	m_text.setFont(*m_font.get());
}

void UIText::Render(sf::RenderTarget& target, sf::RenderStates& states)
{
	target.draw(m_text, states);
}