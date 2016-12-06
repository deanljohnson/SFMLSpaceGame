#pragma once
#include <UI/GameWindow.h>

class MainMenu : public GameWindow
{
private:
	std::function<void()> m_startGameCallback;

	void OnStartGame();
public:
	MainMenu();

	virtual void Update() override;
	
	void SetStartGameCallback(std::function<void()> startGameCallback);
};