#pragma once
#include <UI/GameWindow.h>
#include <SFGUI\Label.hpp>
#include <SFGUI\Button.hpp>

class ConfirmationDialog : public GameWindow 
{
private:
	std::function<void(bool)> m_callback;

	sfg::Label::Ptr m_label;
	sfg::Button::Ptr m_yesButton;
	sfg::Button::Ptr m_noButton;

	void CallCallback(bool result);
public:
	ConfirmationDialog();

	void SetCallback(std::function<void(bool)> callback);
	void SetText(const std::string& text,
				const std::string& yes = "Yes",
				const std::string& no = "No");
};