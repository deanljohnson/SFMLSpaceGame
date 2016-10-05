#pragma once
#include "UIElement.h"
#include <SFML/Graphics/Texture.hpp>
#include <memory>
#include <SFML/Graphics/Sprite.hpp>
#include <ResourceLoader.h>
#include "UITransform.h"

class UIButton : public UIElement
{
private:
	std::shared_ptr<sf::Texture> m_tex;
	sf::Sprite m_sprite;

	enum ButtonState{ None, Hover, Click } state;

	void SwitchState(ButtonState newState);
	UIEventResponse HandleMouse(const sf::Vector2f& localMousePos, UI_Result* resultTarget);

protected:
	virtual sf::FloatRect GetBounds() override;

public:
	explicit UIButton(ResourceID texID, UITransform trans = UITransform());

	virtual UIEventResponse HandleEvent(const sf::Event& event, const sf::Transform& transform, UI_Result* resultTarget) override;

	virtual void Render(sf::RenderTarget& target, sf::RenderStates states) override;

	virtual void UpdateResult(UI_Result* resultTarget) override;
};