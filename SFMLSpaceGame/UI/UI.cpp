#include "stdafx.h"
#include <UI/UI.h>

UI* UI::Singleton{nullptr};

UI::UI()
{
	Singleton = this;
	m_desktop.SetProperty("Entry#invalid", "Color", sf::Color(255, 0, 0));
}

void UI::HandleEvent(const sf::Event& event)
{
	m_desktop.HandleEvent(event);
}

void UI::Update(float dt)
{
	m_desktop.Update(dt);
}

void UI::Display(sf::RenderWindow& target)
{
	m_sfgui.Display(target);
}

void UI::Add(std::shared_ptr<sfg::Widget> widget)
{
	m_desktop.Add(widget);
}

void UI::Remove(std::shared_ptr<sfg::Widget> widget)
{
	m_desktop.Remove(widget);
}

void UI::BringToFront(std::shared_ptr<sfg::Widget> widget)
{
	m_desktop.BringToFront(widget);
}

void UI::SetSize(const sf::Vector2u& size)
{
	m_size = size;
}

sf::Vector2u UI::GetSize() const
{
	return m_size;
}