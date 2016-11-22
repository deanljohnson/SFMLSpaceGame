#pragma once
#include <UI/GameWindow.h>

class StringEntryPopup : public GameWindow
{
private:
	std::function<void(const std::string&)> m_callback;

	void CallCallback(const std::string& name);
public:
	explicit StringEntryPopup(const std::string& key, const std::string& title);

	void SetCallback(std::function<void(const std::string&)> callback);
};