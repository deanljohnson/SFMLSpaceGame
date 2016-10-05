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

	virtual UIEventResponse HandleEvent(const sf::Event& event, const sf::Transform& transform, UI_Result* resultTarget) override;

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
};