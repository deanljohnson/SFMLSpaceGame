#pragma once
#include "GameState.h"

class InitialGameState : public GameState
{
private:
public:
	virtual void Init() const override;
	virtual void CleanUp() const override;

	virtual void Pause() const override;
	virtual void Resume() const override;

	virtual void ProcessEvent(const sf::Event& event) const override;
	virtual void Update() const override;
	virtual void Render(sf::RenderTarget& target) override;
};