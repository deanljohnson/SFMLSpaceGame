#pragma once
#include "GameState.h"
#include "EntityManager.h"

class InitialGameState : public GameState
{
private:
	EntityManager m_entityManager;

public:
	virtual void Init() override;
	virtual void CleanUp() const override;

	virtual void Pause() const override;
	virtual void Resume() const override;

	virtual void ProcessEvent(const sf::Event& event) const override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};