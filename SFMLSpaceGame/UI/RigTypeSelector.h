#pragma once
#include <UI/GameWindow.h>

class RigTypeSelector : public GameWindow
{
private:
	std::function<void(const std::string&)> m_callback;

	void CallCallback(const std::string& name);
public:
	RigTypeSelector();

	void SetCallback(std::function<void(const std::string&)> callback);
};