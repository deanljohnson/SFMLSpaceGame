#pragma once
#include <SFML/Graphics/Transform.hpp>
#include <UI/UIEventResponse.h>
#include <UI/UI_Result.h>
#include <SFML/Graphics/Transformable.hpp>

namespace sf{
	class Event;
	class RenderStates;
	class RenderTarget;
}

class UIElement : public sf::Transformable
{
public:
	UIElement(){}
	virtual ~UIElement()
	{
	}

	virtual UIEventResponse HandleEvent(const sf::Event& event, const sf::Transform& transform, UI_Result* resultTarget) { return None; }

	virtual void Refresh() {}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) = 0;
	virtual sf::FloatRect GetBounds() = 0;

	// Used for resetting result state and such. Called after rendering
	virtual void UpdateResult(UI_Result* resultTarget) {}
};