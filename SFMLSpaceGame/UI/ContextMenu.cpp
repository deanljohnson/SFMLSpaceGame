#include "stdafx.h"
#include <UI/ContextMenu.h>

ContextMenu::ContextMenu() 
	: GameWindow("context_menu"),
	  m_groupOpen(false)
{
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_window->SetTitle("Options");
	m_window->SetId("context_menu");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	m_optionBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Context menus are automatically hidden 
	// when the mouse leaves
	m_window->GetSignal(sfg::Window::OnMouseLeave).Connect(
		[this]() { if (!m_groupOpen) Show(false); });

	m_window->Add(m_optionBox);
}

sfg::Button::Ptr ContextMenu::AddOption(const std::string& option, Callback callback, sfg::Box::Ptr box)
{
	auto b = sfg::Button::Create(option);

	b->GetSignal(sfg::Button::OnLeftClick).Connect(
		[callback] { if (callback != nullptr) callback(); });

	box->Pack(b);

	return b;
}

void ContextMenu::AddOption(const std::string& option, Callback callback) 
{
	AddOption(option, callback, m_optionBox);
}

void ContextMenu::AddOption(const Option& option) 
{
	AddOption(option.first, option.second, m_optionBox);
}

void ContextMenu::AddGroup(const std::string& groupName, 
							std::initializer_list<Option> options) 
{
	AddGroup(groupName, options);
}

void ContextMenu::AddGroup(const std::string& groupName, const std::vector<Option>& options)
{
	// Create the grouping
	auto groupWindow = sfg::Window::Create(sfg::Window::BACKGROUND);
	auto groupBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	// Add options to the group
	for (auto o : options)
	{
		AddOption(o.first, o.second, groupBox);
	}

	m_groups.push_back(groupWindow);
	groupWindow->Add(groupBox);

	groupWindow->Show(false);

	std::weak_ptr<sfg::Window> groupWindowWP{ groupWindow };

	auto groupButton = AddOption(groupName + "->", [this, groupWindowWP]()
	{
		if (groupWindowWP.expired())
			throw "Ptr to group window expired in context menu";
		auto gw = groupWindowWP.lock();
		// Place group popup at mouse
		gw->SetPosition(GetScreenMouseLocation() - sf::Vector2f{ 5,5 });
		gw->Show(true);
		m_groupOpen = true;
		// Make sure the popup is not covered
		UI::Singleton->BringToFront(gw);
	}, m_optionBox);

	groupWindow->GetSignal(sfg::Window::OnMouseLeave).Connect([this, groupWindowWP]
	{
		if (groupWindowWP.expired())
			throw "Ptr to group window expired in context menu";
		auto gw = groupWindowWP.lock();
		m_groupOpen = false;
		gw->Show(false);
		// If the mouse is now out of the context menu, hide the context menu
		if (!WidgetHelpers::MouseInWidget(m_window))
			Show(false);
	});

	// Group windows needs to be separate from the 
	UI::Singleton->Add(groupWindow);
}

void ContextMenu::ClearOptions() 
{
	m_optionBox->RemoveAll();

	for (auto g : m_groups) 
	{
		UI::Singleton->Remove(g);
	}
	m_groups.clear();
}