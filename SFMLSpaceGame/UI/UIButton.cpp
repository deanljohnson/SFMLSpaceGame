#include <UI/UI.h>
#include <UI/UIButton.h>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

UIButton::UIButton(ResourceID texID, UITransform trans)
{
	m_tex = LoadTextureResource(texID);

	setPosition(trans.position);
	setScale(trans.scale);

	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tex->getSize().x, m_tex->getSize().y / 3));
}

sf::FloatRect UIButton::GetBounds()
{
	return getTransform().transformRect(m_sprite.getGlobalBounds());
}

void UIButton::SwitchState(ButtonState newState)
{
	if (state == newState) return;
	int stateHeight = m_tex->getSize().y / 3;

	// Kind of a hack here, but reduces the branching.
	// Equivalent, non-hack code is commented below
	m_sprite.setTextureRect(sf::IntRect(0, stateHeight * newState, m_tex->getSize().x, stateHeight));

	/*if (newState == Hover)
	{
		// Change the graphic in use
		m_sprite.setTextureRect(sf::IntRect(0, stateHeight, m_tex->getSize().x, stateHeight));
	}
	else if (newState == Click)
	{
		m_sprite.setTextureRect(sf::IntRect(0, stateHeight * 2, m_tex->getSize().x, stateHeight));
	}
	else
	{
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_tex->getSize().x, stateHeight));
	}*/

	state = newState;
}

UIEventResponse UIButton::HandleMouse(const sf::Vector2f& localMousePos, UI_Result* resultTarget)
{
	if (getTransform().transformRect(m_sprite.getGlobalBounds()).contains(localMousePos))
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// Switch to clicked texture
			SwitchState(Click);
		}
		else
		{
			resultTarget->booleanValue = (state == Click);

			// Switch to hover texture
			SwitchState(Hover);
		}

		return UIEventResponse::Consume;
	}

	SwitchState(None);
	return UIEventResponse::None;
}

UIEventResponse UIButton::HandleEvent(const sf::Event& event, const sf::Transform& transform)
{
	UIEventResponse response = UIEventResponse::None;

	if (event.type == sf::Event::MouseMoved)
	{
		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));

		// Convert mousePos to local coordinates
		mousePos = transform.getInverse().transformPoint(mousePos);

		response = HandleMouse(mousePos, UI::GetResult(ID));
	}
	if (event.type == sf::Event::MouseButtonPressed
		|| event.type == sf::Event::MouseButtonReleased)
	{
		sf::Vector2f mousePos = sf::Vector2f(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));

		// Convert mousePos to local coordinates
		mousePos = transform.getInverse().transformPoint(mousePos);

		response = HandleMouse(mousePos, UI::GetResult(ID));
	}
	
	if (response != UIEventResponse::Consume) 
	{
		UIEventResponse childResponse = UIElement::HandleEvent(event, transform);
		if (childResponse == UIEventResponse::None) return response;
		return childResponse;
	}
	return response;
}

void UIButton::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.transform *= getTransform();
	target.draw(m_sprite, states);

	for (auto elem : children) {
		elem->Render(target, states);
	}
}

void UIButton::UpdateResult(UI_Result* resultTarget)
{
	resultTarget->booleanValue = false;
}

