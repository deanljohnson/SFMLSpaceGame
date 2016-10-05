#pragma once
#include "UIElement.h"
#include <string>
#include <ResourceLoader.h>
#include <SFML/Graphics/Text.hpp>
#include "UITransform.h"

class UIText : public UIElement
{
private:
	std::shared_ptr<sf::Font> m_font;
	sf::Text m_text;

protected:
	virtual sf::FloatRect GetBounds() override;

public:
	UIText(std::string text, ResourceID fontID, UITransform trans = UITransform());

	void Refresh(std::string text, ResourceID fontID);

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};