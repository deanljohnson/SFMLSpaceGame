#pragma once
#include "UIElement.h"
#include <ResourceLoader.h>
#include <SFML/Graphics/Text.hpp>
#include "UITransform.h"

struct UITextOption
{
	UITextOption()
		: text(""),
		  fontID(NullResource),
		  color(sf::Color::White),
		  fontSize(20)
	{}

	explicit UITextOption(std::string _text, ResourceID _id = NullResource, sf::Color _color = sf::Color::White, size_t _fontSize = 20)
		: text(_text),
		  fontID(_id),
		  color(_color),
		  fontSize(_fontSize)
	{}

	std::string text;
	ResourceID fontID;
	sf::Color color;
	size_t fontSize;
};

class UIText : public UIElement
{
private:
	ResourceID m_fontID;
	std::shared_ptr<sf::Font> m_font;
	sf::Text m_text;

protected:
	virtual sf::FloatRect GetBounds() override;

public:
	UIText(std::string text, ResourceID fontID, size_t fontSize = 12, UITransform trans = UITransform());
	explicit UIText(UITextOption option, UITransform trans = UITransform());

	void Refresh(std::string text, ResourceID fontID);
	void Refresh(std::string text);
	virtual void Refresh() override {}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};