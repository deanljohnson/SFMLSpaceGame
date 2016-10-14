#include <UI/UI.h>

UI* UI::Singleton{nullptr};

UI::UI()
{
	Singleton = this;

	m_centerAlignmentBox = sfg::Box::Create();

	m_screenWindow = sfg::Window::Create(sfg::Window::NO_STYLE);
	m_screenWindow->Add(m_centerAlignmentBox);

	// Hiding this keeps it from capturing input
	m_centerAlignmentBox->Show(false);

	m_desktop.Add(m_centerAlignmentBox);
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

void UI::Add(std::shared_ptr<sfg::Widget> widget, bool centered)
{
	if (!centered)
	{
		m_desktop.Add(widget);
	}
	else
	{
		m_centerAlignmentBox->Pack(widget, true, true);
	}
}

void UI::Remove(std::shared_ptr<sfg::Widget> widget, bool centered)
{
	if (!centered)
	{
		m_desktop.Remove(widget);
	}
	else
	{
		m_centerAlignmentBox->Remove(widget);
	}
}

void UI::SetSize(const sf::Vector2u& size)
{
	m_size = size;
	m_screenWindow->SetRequisition(sf::Vector2f(m_size.x, m_size.y));
	m_screenWindow->SetAllocation(sf::FloatRect(0, 0, m_size.x, m_size.y));
}

sf::Vector2u UI::GetSize()
{
	return m_size;
}