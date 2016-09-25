#pragma once
#include "GameState.h"
#include "EntityManager.h"
#include <Box2D/Dynamics/b2World.h>

class InitialGameState : public GameState
{
private:
	EntityManager m_entityManager;
	b2World m_world;

public:
	InitialGameState() 
		: m_entityManager(), 
		  m_world(b2Vec2(0, 0))
	{
	}

	virtual void Init() override;
	virtual void CleanUp() const override;

	virtual void Pause() const override;
	virtual void Resume() const override;

	virtual void ProcessEvent(const sf::Event& event) const override;
	virtual void Update() override;
	virtual void Render(sf::RenderTarget& target) override;
};