#include <SpriteHelpers.h>
#include <OriginOption.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

void SpriteHelpers::SetOrigin(sf::Sprite* s, OriginOption option)
{
	switch (option)
	{
	case OriginOption::Center:
		s->setOrigin(sf::Vector2f(s->getLocalBounds().width / 2.f, s->getLocalBounds().height / 2.f));
		break;
	case OriginOption::MiddleRight:
		s->setOrigin(sf::Vector2f(s->getLocalBounds().width, s->getLocalBounds().height / 2.f));
		break;
	case OriginOption::MiddleLeft:
		s->setOrigin(sf::Vector2f(0, s->getLocalBounds().height / 2.f));
		break;
	}
}

sf::Vector2f SpriteHelpers::GetOrigin(const sf::IntRect& rect, OriginOption option)
{
	switch (option)
	{
	case OriginOption::Center:
		return sf::Vector2f(rect.width / 2.f, rect.height / 2.f);
	case OriginOption::MiddleRight:
		return sf::Vector2f(rect.width, rect.height / 2.f);
	case OriginOption::MiddleLeft:
		return sf::Vector2f(0, rect.height / 2.f);
	}
	return {};
}
