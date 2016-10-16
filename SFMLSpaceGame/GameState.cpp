#include <GameState.h>

std::vector<std::unique_ptr<sf::Event>> GameState::pendingEvents{};
EventQueue GameState::pendingGameEvents{};
bool GameState::playerInputLocked{ false };
std::string GameState::playerShipName{ "Alien-One" };

b2World GameState::world{b2Vec2(0,0)};