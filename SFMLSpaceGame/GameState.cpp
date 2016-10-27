#include "stdafx.h"
#include <GameState.h>

std::vector<std::unique_ptr<sf::Event>> GameState::pendingEvents{};
EventQueue GameState::pendingGameEvents{};
bool GameState::playerInputLocked{ false };

b2World GameState::world{b2Vec2(0,0)};