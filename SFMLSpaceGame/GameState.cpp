#include <GameState.h>

std::vector<std::unique_ptr<sf::Event>> GameState::pendingEvents{};
b2World GameState::world{b2Vec2(0,0)};