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