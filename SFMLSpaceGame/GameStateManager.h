#pragma once
#include <memory>
#include <vector>

class GameState;

namespace sf{
	class RenderTarget;
	class Event;
}

class GameStateManager
{
private:
	std::vector<std::unique_ptr<GameState>> m_states;
	std::unique_ptr<GameState> m_nextState;

public:
	static GameStateManager* Singleton;

	GameStateManager();

	void Init();
	void CleanUp();

	void ProcessEvent(const sf::Event& event) const;
	void Update();
	void Render(sf::RenderTarget& target) const;

	void SetNextState(std::unique_ptr<GameState> s);
	void ChangeState(std::unique_ptr<GameState> s);
	void PushState(std::unique_ptr<GameState> s);
	void PopState();
};