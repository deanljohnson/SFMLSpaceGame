#include <UI/UIButton.h>

UIButton::UIButton(ResourceID texID, UITransform trans)
{
	m_tex = LoadTextureResource(texID);

	m_sprite = sf::Sprite(*m_tex.get());
	m_sprite.setTextureRect(sf::IntRect(0, 0, m_tex->getSize().x, m_tex->getSize().y / 3));
	m_sprite.setPosition(trans.position);
	m_sprite.setScale(trans.scale);
}

void UIButton::Render(sf::RenderTarget& target, sf::RenderStates states)
{
	target.draw(m_sprite, states);
}
