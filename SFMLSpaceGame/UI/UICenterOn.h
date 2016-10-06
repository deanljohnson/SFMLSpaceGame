#pragma once
#include "UIElement.h"
#include "UI.h"

class UICenterOn : public UIElement
{
private:
	sf::FloatRect m_targetBounds;
	UI_ID m_targetID;
public:
	explicit UICenterOn(UI_ID targetID)
		: m_targetID(targetID)
	{
		
	}

	virtual UIEventResponse HandleEvent(const sf::Event& event, const sf::Transform& transform) override;

	void Refresh(UI_ID target)
	{
		m_targetID = target;
		if (m_targetID != UI_ID_NULL)
			m_targetBounds = UI::Get(m_targetID)->GetBounds();
	}

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;
	virtual sf::FloatRect GetBounds() override;
};