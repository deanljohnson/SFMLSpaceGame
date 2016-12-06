#pragma once
#include <UI/GameWindow.h>

class MainMenu : public GameWindow
{
private:
	std::function<void()> m_startGameCallback;
	std::function<void()> m_exitCallback;

	void OnStartGame();
	void OnExit();
public:
	MainMenu();

	void SetStartGameCallback(std::function<void()> startGameCallback);
	void SetExitCallback(std::function<void()> exitCallback);
};