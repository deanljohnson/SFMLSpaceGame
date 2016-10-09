#include <GameState.h>

std::vector<std::unique_ptr<sf::Event>> GameState::pendingEvents{};
EventQueue GameState::pendingGameEvents{};

b2World GameState::world{b2Vec2(0,0)};