#include <UI/UIVerticalGroup.h>
#include <SFML/Graphics/RenderStates.hpp>
#include <ExtendedMath.h>

sf::FloatRect UIVerticalGroup::GetBounds()
{
	sf::FloatRect rect = sf::FloatRect(getPosition(), sf::Vector2f());
	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		rect = Union(bounds, rect);
	}

	return rect;
}

void UIVerticalGroup::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		elem->Render(target, states);
		states.transform.translate(0.f, bounds.height + bounds.top);
	}
}