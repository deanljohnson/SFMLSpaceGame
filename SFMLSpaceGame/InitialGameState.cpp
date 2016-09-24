#include "InitialGameState.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>

//nasty global variable is only for testing
sf::CircleShape shape(100.f);

void InitialGameState::Init() const
{
	shape.setFillColor(sf::Color::Green);
}

void InitialGameState::CleanUp() const
{
	
}

void InitialGameState::Pause() const
{
	
}

void InitialGameState::Resume() const
{
	
}

void InitialGameState::ProcessEvent(const sf::Event& event) const
{
	
}

void InitialGameState::Update() const
{
	
}

void InitialGameState::Render(sf::RenderTarget& target)
{
	target.draw(shape);
}
