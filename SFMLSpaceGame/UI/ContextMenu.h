#pragma once
#include <UI/GameWindow.h>

class ContextMenu : public GameWindow 
{
public:
	typedef std::function<void()> Callback;
	typedef std::pair<std::string, Callback> Option;
private:
	bool m_groupOpen;

	// storage for groups, simply to make
	// sure their shared_ptr's remain in scope
	std::vector<sfg::Window::Ptr> m_groups;

	sfg::Box::Ptr m_optionBox;

	sfg::Button::Ptr AddOption(const std::string& option, Callback callback, sfg::Box::Ptr box);
public:
	ContextMenu();

	// Adds the given option with the given callback
	void AddOption(const std::string& option, Callback callback);
	void AddOption(const Option& option);

	// Add the given options under an expanding option
	void AddGroup(const std::string& groupName, std::initializer_list<Option> options);

	// Removes all options from the menu
	void ClearOptions();
};