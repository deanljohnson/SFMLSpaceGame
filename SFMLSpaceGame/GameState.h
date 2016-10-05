#pragma once
#include <vector>
#include <memory>
#include <SFML/Window/Event.hpp>

namespace sf{
	class RenderTarget;
}

class GameState
{
public:
	static std::vector<std::unique_ptr<sf::Event>> pendingEvents;
	virtual ~GameState()
	{
	}

	virtual void Init() = 0;
	virtual void CleanUp() const = 0;

	virtual void Pause() const = 0;
	virtual void Resume() const = 0;

	virtual void ProcessEvent(const sf::Event& event) const = 0;
	virtual void Update() = 0;
	virtual void Render(sf::RenderTarget& target) = 0;
};