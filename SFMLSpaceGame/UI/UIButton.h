#pragma once
#include "UIElement.h"
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <ResourceLoader.h>
#include "UITransform.h"

class UIButton : public UIElement
{
private:
	std::shared_ptr<sf::Texture> m_tex;
	sf::Sprite m_sprite;

public:
	explicit UIButton(ResourceID texID, UITransform trans = UITransform());

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};