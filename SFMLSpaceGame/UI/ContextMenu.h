#pragma once
#include <UI/GameWindow.h>

class ContextMenu : public GameWindow 
{
public:
	typedef std::function<void()> Callback;
private:
	std::vector<Callback> m_callbacks;
	sfg::Box::Ptr m_optionBox;

	void AddOption(const std::string& option);
	void CallCallback(int i);
public:
	ContextMenu();

	// Sets the displayed options for the ContextMenu
	// Clears any existing callbacks and options
	void SetOptions(std::initializer_list<std::string> options);

	// Assigns a callback to a certain option in the menu
	void SetCallback(int index, Callback callback);

	// Adds the given option with the given callback
	void AddOption(const std::string& option, Callback callback);
};