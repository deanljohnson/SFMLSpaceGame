#include "stdafx.h"
#include <UI/ContextMenu.h>

ContextMenu::ContextMenu() 
	: GameWindow("context_menu")
{
	m_window = sfg::Window::Create(sfg::Window::BACKGROUND);
	m_window->SetTitle("Options");
	m_window->SetId("context_menu");
	UI::Singleton->Add(m_window);
	SetupWindowSignals();

	m_optionBox = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);

	m_window->Add(m_optionBox);
}

void ContextMenu::AddOption(const std::string& option) 
{
	auto b = sfg::Button::Create(option);
	int index = m_optionBox->GetChildren().size();

	b->GetSignal(sfg::Button::OnLeftClick).Connect(
		[this, index]() 
		{
			CallCallback(index);
		});

	m_optionBox->Pack(b);
}

void ContextMenu::CallCallback(int index) 
{
	if (m_callbacks[index] != nullptr) 
	{
		m_callbacks[index]();
	}
}

void ContextMenu::SetOptions(std::initializer_list<std::string> options) 
{
	m_callbacks.clear();
	m_callbacks.resize(options.size());
	m_optionBox->RemoveAll();

	for (const auto& s : options) 
	{
		AddOption(s);
	}
}

void ContextMenu::SetCallback(int index, Callback callback) 
{
	m_callbacks[index] = callback;
}

void ContextMenu::AddOption(const std::string& option, Callback callback) 
{
	AddOption(option);
	m_callbacks.push_back(callback);
}