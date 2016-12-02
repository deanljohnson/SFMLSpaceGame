#pragma once
#include <GameState.h>
#include <UI/MainMenu.h>

class MainMenuState : public GameState
{
private:
	MainMenu m_mainMenu;
public:
	MainMenuState();

	virtual void Init() override;
	virtual void CleanUp() override;

	virtual void Pause() override;
	virtual void Resume() override;

	virtual void ProcessEvent(const sf::Event& event) const override;

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};