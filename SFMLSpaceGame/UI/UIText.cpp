#include <UI/UIText.h>
#include <SFML/Graphics/RenderTarget.hpp>

UIText::UIText(std::string text, ResourceID fontID, size_t fontSize, UITransform trans)
{
	m_fontID = fontID;
	m_font = LoadFont(fontID);

	setPosition(trans.position);
	setScale(trans.scale);

	m_text.setFont(*m_font.get());
	m_text.setString(text);
	m_text.setFillColor(sf::Color::White);
	m_text.setCharacterSize(fontSize);
}

UIText::UIText(UITextOption option, UITransform trans)
	: UIText(option.text, option.fontID, option.fontSize, trans)
{
	m_text.setFillColor(option.color);
}

sf::FloatRect UIText::GetBounds()
{
	return getTransform().transformRect(m_text.getGlobalBounds());
}

void UIText::Refresh(std::string text, ResourceID fontID)
{
	m_text.setString(text);

	if (fontID == m_fontID) return;
	m_fontID = fontID;
	m_font = LoadFont(m_fontID);
	m_text.setFont(*m_font.get());
}

void UIText::Refresh(std::string text)
{
	m_text.setString(text);
}

void UIText::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform *= getTransform();
	target.draw(m_text, states);

	for (auto elem : children) {
		elem->Render(target, states);
	}
}