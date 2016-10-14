#pragma once
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Alignment.hpp>

namespace sf{
	class RenderWindow;
	class Event;
}

class UI
{
private:
	sfg::SFGUI m_sfgui;
	sfg::Desktop m_desktop;
	sf::Vector2u m_size;
public:
	static UI* Singleton;

	UI();

	void HandleEvent(const sf::Event& event);
	void Update(float dt);
	void Display(sf::RenderWindow& target);
	void Add(std::shared_ptr<sfg::Widget> widget, bool centered = false);
	void Remove(std::shared_ptr<sfg::Widget> widget, bool centered = false);

	void SetSize(const sf::Vector2u& size);
	sf::Vector2u GetSize();
};