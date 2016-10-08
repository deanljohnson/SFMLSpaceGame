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

	return getTransform().transformRect(rect);
}

UIEventResponse UIVerticalGroup::HandleEvent(const sf::Event& event, const sf::Transform& transform)
{
	UIEventResponse response = UIEventResponse::None;
	UIEventResponse childResponse = UIEventResponse::None;
	sf::Transform trans = transform * getTransform();

	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		childResponse = elem->HandleEvent(event, trans);
		trans.translate(0, bounds.height + bounds.top);

		if (childResponse == UIEventResponse::Consume)
		{
			response = UIEventResponse::Consume;
			break;
		}
		else if (childResponse == UIEventResponse::PassOn)
		{
			response = UIEventResponse::PassOn;
		}
	}
	return response;
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