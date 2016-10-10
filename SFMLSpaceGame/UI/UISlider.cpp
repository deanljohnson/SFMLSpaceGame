#include <UI/UISlider.h>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

UISlider::UISlider(float barOffsetX, float barOffsetY, ResourceID texID, UITransform trans)
	: m_barOffset(barOffsetX, barOffsetY)
{
	m_tex = LoadTextureResource(texID);
	sf::Vector2u texSize = m_tex->getSize();

	setPosition(trans.position);
	setScale(trans.scale);

	m_backSprite = sf::Sprite(*m_tex.get());
	m_fill = sf::RectangleShape(sf::Vector2f(texSize.x - barOffsetX * 2, texSize.y - barOffsetY * 2));
	m_fill.setFillColor(sf::Color::Red);
	m_fill.setPosition(barOffsetX, barOffsetY + (texSize.y - barOffsetY * 2) / 2);
	m_fill.setOrigin(0, (texSize.y - barOffsetY * 2) / 2);
}

void UISlider::SetValue(float val)
{
	// clamp val to [0, 1]
	val = std::max(0.f, std::min(1.f, val));
	m_fill.setScale(val, 1);
}

void UISlider::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	states.texture = m_tex.get();
	states.transform *= getTransform();
	target.draw(m_backSprite, states);
	target.draw(m_fill, states);

	for (auto elem : children) {
		elem->Render(target, states);
	}
}

sf::FloatRect UISlider::GetBounds()
{
	return getTransform().transformRect(m_backSprite.getLocalBounds());
}