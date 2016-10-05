#pragma once
#include "UIElement.h"
#include <SFML/Graphics/VertexArray.hpp>
#include <ResourceLoader.h>
#include <UI/UITransform.h>

class UIExpandingBackground : public UIElement
{
private:
	std::shared_ptr<sf::Texture> m_tex;
	sf::VertexArray m_vertArray;
	
public:
	explicit UIExpandingBackground(ResourceID texID, sf::Vector2i size, UITransform trans = UITransform());

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};