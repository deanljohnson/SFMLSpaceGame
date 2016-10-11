#pragma once
#include "UIElement.h"
#include "UITransform.h"
#include <ResourceLoader.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class UISlider : public UIElement
{
private:
	std::shared_ptr<sf::Texture> m_tex{ nullptr };
	sf::Sprite m_backSprite;
	sf::Vector2f m_barOffset;
	sf::RectangleShape m_fill;

public:
	UISlider(float barOffsetX, float barOffsetY, ResourceID texID, UITransform trans = UITransform());
	
	void SetValue(float val);
	virtual void Refresh(float val);

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	virtual sf::FloatRect GetBounds() override;
};