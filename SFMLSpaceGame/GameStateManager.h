#pragma once
#include "GameState.h"
#include <stack>

namespace sf{
	class RenderTarget;
	class Event;
}

class GameStateManager
{
private:
	std::stack<GameState*> m_states;

public:
	void Init();
	void CleanUp() const;

	void ProcessEvent(const sf::Event& event) const;
	void Update() const;
	void Render(sf::RenderTarget& target) const;

	void ChangeState(GameState* s);
	void PushState(GameState* s);
	void PopState();
};