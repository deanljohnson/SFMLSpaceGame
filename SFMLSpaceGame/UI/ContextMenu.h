#pragma once
#include <UI/GameWindow.h>

class ContextMenu : public GameWindow 
{
public:
	typedef std::function<void()> Callback;

	struct Option
	{
		Option(const std::string& opt, Callback click, Callback hovStart = nullptr, Callback hovEnd = nullptr);
		std::string option;
		Callback onClick;
		Callback onHoverStart;
		Callback onHoverEnd;
	};

private:
	bool m_groupOpen;

	// storage for groups, simply to make
	// sure their shared_ptr's remain in scope
	std::vector<sfg::Window::Ptr> m_groups;

	sfg::Box::Ptr m_optionBox;

	sfg::Button::Ptr AddOption(const Option& option, sfg::Box::Ptr box);

	void OpenGroupWindow(std::weak_ptr<sfg::Window> gw);
	void CloseGroupWindow(std::weak_ptr<sfg::Window> gw);
public:
	ContextMenu();

	// Adds the given option
	void AddOption(const Option& option);

	// Add the given options under an expanding option
	void AddGroup(const std::string& groupName, std::initializer_list<Option> options);
	void AddGroup(const std::string& groupName, const std::vector<Option>& options);

	// Removes all options from the menu
	void ClearOptions();

	void Close();
};