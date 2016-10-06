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

UIEventResponse UIHorizontalGroup::HandleEvent(const sf::Event& event, const sf::Transform& transform) 
{
	UIEventResponse response = None;
	UIEventResponse childResponse = None;
	sf::Transform trans = transform * getTransform();

	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		childResponse = elem->HandleEvent(event, trans);
		trans.translate(bounds.width + bounds.left, 0.f);

		if (childResponse == Consume)
		{
			response = Consume;
			break;
		}
		else if (childResponse == PassOn)
		{
			response = PassOn;
		}
	}
	return response;
}

void UIHorizontalGroup::Render(sf::RenderTarget& target, sf::RenderStates states) 
{
	for (auto elem : children) 
	{
		auto bounds = elem->GetBounds();
		elem->Render(target, states);
		states.transform.translate(bounds.width + bounds.left, 0.f);
	}
}