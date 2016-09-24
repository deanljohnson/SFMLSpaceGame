#pragma once

namespace sf{
	class RenderTarget;
	class Event;
}

class GameState
{
public:
	virtual ~GameState()
	{
	}

	virtual void Init() const = 0;
	virtual void CleanUp() const = 0;

	virtual void Pause() const = 0;
	virtual void Resume() const = 0;

	virtual void ProcessEvent(const sf::Event& event) const = 0;
	virtual void Update() const = 0;
	virtual void Render(sf::RenderTarget& target) = 0;
};
