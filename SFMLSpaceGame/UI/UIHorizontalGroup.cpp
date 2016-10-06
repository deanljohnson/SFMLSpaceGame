#include <UI/UIHorizontalGroup.h>
#include <SFML/Graphics/RenderStates.hpp>
#include <ExtendedMath.h>

sf::FloatRect UIHorizontalGroup::GetBounds()
{
	sf::FloatRect rect = sf::FloatRect(getPosition(), sf::Vector2f());
	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		rect = Union(bounds, rect);
	}

	return rect;
}

void UIHorizontalGroup::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	for (auto elem : children) 
	{
		auto bounds = elem->GetBounds();
		elem->Render(target, states);
		states.transform.translate(bounds.width + bounds.left, 0.f);
	}

	children.clear();
}