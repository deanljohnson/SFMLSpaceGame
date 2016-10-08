#pragma once
#include <GameState.h>
#include <EntityManager.h>
#include <PhysicsStepper.h>
#include "ContactFilter.h"

class InitialGameState : public GameState
{
private:
	PhysicsStepper m_stepper;
	ContactFilter m_contactListener;

public:
	InitialGameState() 
		: m_stepper()
	{}

	virtual void Init() override;
	virtual void CleanUp() const override;

	virtual void Pause() const override;
	virtual void Resume() const override;

	virtual void ProcessEvent(const sf::Event& event) const override;

	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};