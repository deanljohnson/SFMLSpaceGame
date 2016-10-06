#include <UI/UIArea.h>
#include <SFML/Graphics/RenderStates.hpp>

sf::FloatRect UIArea::GetBounds()
{
	return getTransform().transformRect(m_rect);
}

void UIArea::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform.combine(getTransform());
	for (auto child : children) {
		child->Render(target, states);
	}
}
