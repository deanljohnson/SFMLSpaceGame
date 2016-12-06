#pragma once
#include <GameState.h>
#include <PhysicsStepper.h>
#include <ContactFilter.h>

class InitialGameState : public GameState
{
private:
	EntityID m_playerID;
	EntityID m_playerSpawnerID;

	PhysicsStepper m_stepper;
	ContactFilter m_contactListener;

	class InitialGameStateImpl;
	std::unique_ptr<InitialGameStateImpl> m_impl;

	bool m_paused;

	void HandlePause();

public:
	InitialGameState();

	virtual void Init() override;
	virtual void CleanUp() override;

	virtual void Pause() override;
	virtual void Resume() override;

	virtual void ProcessEvent(const sf::Event& event) const override;

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};