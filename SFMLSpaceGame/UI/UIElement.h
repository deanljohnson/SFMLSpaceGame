#pragma once
#include <vector>
#include <SFML/Graphics/Transformable.hpp>
#include <UI/UIEventResponse.h>
#include <UI/UI_Result.h>
#include <UI/UI_ID.h>


namespace sf{
	class Event;
	class RenderStates;
	class RenderTarget;
}

class UIElement : public sf::Transformable
{
public:
	std::vector<UIElement*> children;
	UI_ID ID;

	UIElement(){}
	virtual ~UIElement()
	{
	}

	virtual UIEventResponse HandleEvent(const sf::Event& event, const sf::Transform& transform) 
	{ 
		if (!children.empty()) 
		{
			sf::Transform trans = transform * getTransform();
			UIEventResponse response = UIEventResponse::None;
			UIEventResponse childResponse = UIEventResponse::None;
			for (auto ch : children)
			{
				childResponse = ch->HandleEvent(event, trans);

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
		else return UIEventResponse::None;
	}

	virtual void Refresh() {}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) = 0;
	virtual sf::FloatRect GetBounds() = 0;

	// Used for resetting result state and such. Called after rendering
	virtual void UpdateResult(UI_Result* resultTarget) {}
};