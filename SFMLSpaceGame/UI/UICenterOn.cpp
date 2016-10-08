#include <UI/UICenterOn.h>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <ExtendedMath.h>
#include "UI.h"

sf::FloatRect UICenterOn::GetBounds()
{
	sf::FloatRect rect = sf::FloatRect(getPosition(), sf::Vector2f());
	for (auto elem : children)
	{
		auto bounds = elem->GetBounds();
		rect = Union(bounds, rect);
	}

	return rect;
}

UIEventResponse UICenterOn::HandleEvent(const sf::Event& event, const sf::Transform& transform)
{
	sf::Vector2f targetCenter = sf::Vector2f(m_targetBounds.left + (m_targetBounds.width / 2.f),
											m_targetBounds.top + (m_targetBounds.height / 2.f));

	UIEventResponse response = UIEventResponse::None;
	UIEventResponse childResponse = UIEventResponse::None;
	for (auto child : children) 
	{
		sf::Transform copyTrans = sf::Transform(transform);
		auto childBounds = child->GetBounds();
		sf::Vector2f childCenter = sf::Vector2f(childBounds.left + (childBounds.width / 2.f),
			childBounds.top + (childBounds.height / 2.f));

		copyTrans.translate(targetCenter - childCenter);

		childResponse = child->HandleEvent(event, copyTrans);

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


void UICenterOn::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	sf::Vector2f targetCenter;
	if (m_targetID == UI_ID_NULL)
		m_targetBounds = sf::FloatRect(0, 0, target.getSize().x, target.getSize().y);
	else
		m_targetBounds = UI::Get(m_targetID)->GetBounds();

	targetCenter = sf::Vector2f(m_targetBounds.left + (m_targetBounds.width / 2.f),
								m_targetBounds.top + (m_targetBounds.height / 2.f));

	for (auto child : children)
	{
		sf::RenderStates copyStates = sf::RenderStates(states);
		auto childBounds = child->GetBounds();
		sf::Vector2f childCenter = sf::Vector2f(0 + (childBounds.width / 2.f),
												0 + (childBounds.height / 2.f));

		copyStates.transform.translate(targetCenter - childCenter);

		child->Render(target, copyStates);
	}
}
